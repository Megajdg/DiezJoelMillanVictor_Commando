#include "Enemy.h"
#include "Scene.h"
#include "CircleCollider.h"
#include "MyPhysics.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Game.h"
#include "Parameters.h"
#include "AnimatedEntity.h"
#include "AnimationsEnemy.h"
#include "AudioManager.h"

template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

static std::string DirectionToAnimation(float angle)
{
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;

    if (angle >= 337.5f || angle < 22.5f)
        return "run_front";

    if (angle < 67.5f)
        return "run_diag_front_right";

    if (angle < 112.5f)
        return "run_right";

    if (angle < 157.5f)
        return "run_diag_back_right";

    if (angle < 202.5f)
        return "run_back";

    if (angle < 247.5f)
        return "run_diag_back_left";

    if (angle < 292.5f)
        return "run_left";

    return "run_diag_front_left";
}

static std::string DirectionToIdle(float angle)
{
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;

    // FRONT (0°)
    if (angle >= 315.f || angle < 45.f)
        return "idle_front";

    // RIGHT (90°)
    if (angle < 135.f)
        return "idle_right";

    // BACK (180°)
    if (angle < 225.f)
        return "idle_back";

    // LEFT (270°)
    return "idle_left";
}


Enemy::Enemy(Scene* scene, const Transform& t, Player* target, const std::string& spritesheet)
    : AnimatedEntity(scene, spritesheet, t, Vector2(100, 100))
{

    this->target = target;
    scene->AddActor(this);

    animationSet = LoadEnemyAnimations();
    SetAnimation("idle_front");

    // Collider
    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    scene->mph->AddCollider(col, this);

    // IA inicial
    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    Vector2 pos = transform.position;

    if (pos.x < mapLeft) moveDir = Vector2(1, 0);
    else if (pos.x > mapRight) moveDir = Vector2(-1, 0);
    else if (pos.y < mapTop) moveDir = Vector2(0, 1);
    else if (pos.y > mapBottom) moveDir = Vector2(0, -1);

    if (moveDir.x == 0.f && moveDir.y == 0.f)
    {
        ChooseNewDirection();
    }

    state = EnemyState::MOVING;
    stateTimer = 0.5f + (rand() % 1000) / 1000.f;
}

void Enemy::Update(float dt)
{
    AnimatedEntity::Update(dt);

    if (dying)
    {
        if (currentAnimation->finished)
            myScene->DestroyActor(this);

        return;
    }

    stateTimer -= dt;

    switch (state)
    {
    case EnemyState::MOVING:
        hasShot = false;
        UpdateMoving(dt);
        break;

    case EnemyState::SHOOTING:
        UpdateShooting(dt);
        break;
    }

    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    // Limitar X
    transform.position.x = Clamp(transform.position.x, mapLeft, mapRight);

    // Limitar Y dentro del mapa
    transform.position.y = Clamp(transform.position.y, mapTop, mapBottom);

    // Convertir posición del enemigo a pantalla
    float enemyScreenY = transform.position.y - Game::camera.position.y + Parameters::screenHeight * 0.5f;

    // Top del HUD en pantalla
    float hudTopScreen = Parameters::screenHeight * 0.66f + 30;

    // Si el enemigo entra en el HUD, lo destruimos
    if (enemyScreenY > hudTopScreen)
    {
        myScene->DestroyActor(this);
        return;
    }
}

void Enemy::UpdateMoving(float dt)
{
    // Si acabó el tiempo de movimiento pasar a disparar
    if (stateTimer <= 0.f)
    {
        state = EnemyState::SHOOTING;
        stateTimer = 0.3f + (rand() % 300) / 1000.f;
        return;
    }

    // Si no se mueve  idle
    float len2 = moveDir.x * moveDir.x + moveDir.y * moveDir.y;
    if (len2 < 0.0001f)
    {
        float angle = transform.rotation + 90.f;
        SetAnimation(DirectionToIdle(angle));
        return;
    }

    // Mover
    transform.position += moveDir * speed * dt;
    transform.rotation = atan2(moveDir.y, moveDir.x) * 180.f / 3.14159f;

    // Animación de run según dirección
    float angle = transform.rotation + 90.f;
    SetAnimation(DirectionToAnimation(angle));
}

void Enemy::UpdateShooting(float dt)
{
    if (stateTimer <= 0.f)
    {
        // Elegir nueva dirección de movimiento
        ChooseNewDirection();

        state = EnemyState::MOVING;
        stateTimer = 0.5f + (rand() % 1000) / 1000.f; // 0.5–1.5s
        return;
    }

    // Disparar una vez al entrar en el estado
    if (!hasShot)
    {
        ShootAtPlayer();
        hasShot = true;
        return;
    }

    Vector2 dir = (target->transform.position - transform.position).normalize();
    float angle = atan2(dir.y, dir.x) * 180.f / 3.14159f + 90.f;

    SetAnimation(DirectionToIdle(angle));
}

void Enemy::ChooseNewDirection()
{
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    Vector2 pos = transform.position;

    // Lista de direcciones válidas
    std::vector<Vector2> validDirs;

    for (int i = 0; i < 8; i++)
    {
        Vector2 d = dirs[i].normalize();
        Vector2 nextPos = pos + d * 50.f; // mirar un poco hacia adelante

        bool inside =
            nextPos.x > mapLeft &&
            nextPos.x < mapRight &&
            nextPos.y > mapTop &&
            nextPos.y < mapBottom;

        if (inside)
            validDirs.push_back(d);
    }

    // Si hay direcciones válidas, elegir una aleatoria
    if (!validDirs.empty())
    {
        int i = rand() % validDirs.size();
        moveDir = validDirs[i];
        return;
    }

    // Si no hay válidas (muy raro), elegir la que más acerque al centro del mapa
    Vector2 center(0, -3116); // centro del mapa
    Vector2 bestDir;
    float bestDot = -9999.f;

    Vector2 toCenter = (center - pos).normalize();

    for (int i = 0; i < 8; i++)
    {
        Vector2 d = dirs[i].normalize();
        float dot = d.x * toCenter.x + d.y * toCenter.y;

        if (dot > bestDot)
        {
            bestDot = dot;
            bestDir = d;
        }
    }

    moveDir = bestDir;
}

void Enemy::TakeDamage(int dmg)
{
    if (isDead)
        return;

    health -= dmg;
    if (health <= 0)
    {
        AudioManager::instance().PlaySFX("hit.wav");
        ((GameScene*)myScene)->AddScore(100);
        isDead = true;
        dying = true;
        deathTimer = 1.f;
        SetAnimation("death");
    }
}
