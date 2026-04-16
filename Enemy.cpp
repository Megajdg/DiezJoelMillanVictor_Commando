#include "Enemy.h"
#include "Scene.h"
#include "CircleCollider.h"
#include "MyPhysics.h"
#include "Bullet.h"
#include "GameScene.h"
#include "Game.h"
#include "Parameters.h"

template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

Enemy::Enemy(Scene* scene, const Transform& t, Player* target) : Sprite(scene, "Joe.png", t, Vector2(100, 100))
{
    this->target = target;
    scene->AddActor(this);

    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    scene->mph->AddCollider(col, this);

    // Elegir dirección inicial según dónde ha aparecido
    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    Vector2 pos = transform.position;

    // Si está a la izquierda del mapa mover hacia la derecha
    if (pos.x < mapLeft)
        moveDir = Vector2(1, 0);

    // Si está a la derecha del mapa mover hacia la izquierda
    else if (pos.x > mapRight)
        moveDir = Vector2(-1, 0);

    // Si está por arriba del mapa mover hacia abajo
    else if (pos.y < mapTop)
        moveDir = Vector2(0, 1);

    // Si está por abajo (no debería, pero por seguridad)
    else if (pos.y > mapBottom)
        moveDir = Vector2(0, -1);

    // Primer estado
    state = EnemyState::MOVING;
    stateTimer = 0.5f + (rand() % 1000) / 1000.f;
}

void Enemy::Update(float dt)
{
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

    // Limitar X
    transform.position.x = Clamp(transform.position.x, mapLeft, mapRight);

    float hudHeight = 200;
    float limitBottom = Game::camera.position.y + Parameters::screenHeight * 0.5f - hudHeight + 100;

    // Si el enemigo sale por debajo de la pantalla destruir
    if (transform.position.y > limitBottom)
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
        stateTimer = 0.3f + (rand() % 300) / 1000.f; // 0.3–0.6s
        return;
    }

    // Mover en la dirección elegida
    transform.position += moveDir * speed * dt;
    transform.rotation = atan2(moveDir.y, moveDir.x) * 180.f / 3.14159f;
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
    }
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
    health -= dmg;
    if (health <= 0)
    {
        ((GameScene*)myScene)->AddScore(100);
        myScene->DestroyActor(this);
    }
}
