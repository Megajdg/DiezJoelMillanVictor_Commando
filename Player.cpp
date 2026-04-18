#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GunWeapon.h"
#include "Grenade.h"
#include "GameScene.h"
#include "Parameters.h"
#include "AnimatedEntity.h"
#include "AnimationsPlayer.h"
#include "AudioManager.h"

static std::string DirectionToAnimation(float angle)
{
    // Normalizamos el ángulo a 0–360
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

static std::string IdleFromRun(const std::string& runAnim)
{
    if (runAnim == "run_front") return "idle_front";
    if (runAnim == "run_back") return "idle_back";

    // laterales cualquiera vale
    if (runAnim == "run_left") return "idle_left";
    if (runAnim == "run_right") return "idle_right";

    // diagonales simplificamos
    if (runAnim == "run_diag_front_left") return "idle_left";
    if (runAnim == "run_diag_front_right") return "idle_right";
    if (runAnim == "run_diag_back_left") return "idle_left";
    if (runAnim == "run_diag_back_right") return "idle_right";

    return "idle_front";
}

template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

Player::Player(Scene* myscene, std::string img_name, float side_size) : AnimatedEntity(myscene, img_name, Transform(), Vector2(side_size, side_size))
{
    Game::camera.target = this;
    weapon = new GunWeapon();

    animationSet = LoadPlayerAnimations();
    SetAnimation("idle_front");
}

Player::Player(Scene* myscene, std::string img_name, Transform transform, float side_size) : AnimatedEntity(myscene, img_name, transform, Vector2(side_size, side_size))
{

    Game::camera.target = this;
    weapon = new GunWeapon();

    animationSet = LoadPlayerAnimations();
    SetAnimation("idle_front");
    
}

void Player::Update(float deltaTime)
{
    AnimatedEntity::Update(deltaTime);
    UpdateMovement(deltaTime);

    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    // Limitar X
    transform.position.x = Clamp(transform.position.x, mapLeft, mapRight);

    // Limitar Y dentro del mapa
    transform.position.y = Clamp(transform.position.y, mapTop, mapBottom);

    // Convertir posición del jugador a pantalla
    float playerScreenY = transform.position.y - Game::camera.position.y + Parameters::screenHeight * 0.5f;

    // Top del HUD en pantalla
    float hudTopScreen = Parameters::screenHeight * 0.66f - 70;

    // Si el jugador entra en el HUD, lo empujamos hacia arriba
    if (playerScreenY > hudTopScreen)
    {
        float delta = playerScreenY - hudTopScreen;

        // Convertimos el empuje a coordenadas del mundo
        transform.position.y -= delta;
    }
}

void Player::UpdateMovement(float deltaTime)
{
    if (dying)
    {
        deathTimer -= deltaTime;

        if (deathTimer <= 0.f)
        {
            if (lives > 0)
                Game::OnPlayerDeath(this);
            else
                Game::OnGameOver();
        }

        return;
    }

    Vector2 dir(0, 0);

    bool up = Game::keyDown[SDLK_W] || Game::keyDown[SDLK_UP];
    bool down = Game::keyDown[SDLK_S] || Game::keyDown[SDLK_DOWN];
    bool left = Game::keyDown[SDLK_A] || Game::keyDown[SDLK_LEFT];
    bool right = Game::keyDown[SDLK_D] || Game::keyDown[SDLK_RIGHT];

    if (up)    dir.y -= 1;
    if (down)  dir.y += 1;
    if (left)  dir.x -= 1;
    if (right) dir.x += 1;

    if (dir.x != 0 || dir.y != 0)
    {
        dir = dir.normalize();

        float speed = 300.f;
        transform.position += dir * speed * deltaTime;

        transform.rotation = atan2(dir.y, dir.x) * 180.f / 3.14159f;
    }

    if (isThrowing)
    {
        Animation* a = currentAnimation;

        if (a->finished)
        {
            isThrowing = false;
            SetAnimation(IdleFromRun(lastMoveAnim));
        }
    }

    if (dir.x != 0 || dir.y != 0)
    {
        dir = dir.normalize();

        float speed = 300.f;
        transform.position += dir * speed * deltaTime;

        float angle = atan2(dir.y, dir.x) * 180.f / 3.14159f;
        angle += 90.f;
        transform.rotation = angle;

        std::string anim = DirectionToAnimation(angle);
        SetAnimation(anim);
        lastMoveAnim = anim;

    }
    else
    {
        SetAnimation(IdleFromRun(lastMoveAnim));
    }

    // Disparo
    if (Game::keyDown[SDLK_SPACE])
    {
        AudioManager::instance().playSFX("shoot.wav");
        Game::keyDown[SDLK_SPACE] = false;

        Transform shot;
        shot.position = transform.position;

        shot.rotation = transform.rotation - 90.f;

        weapon->Shoot(myScene, shot);
    }

    if (Game::keyDown[SDLK_G] && !grenadeActive)
    {
        AudioManager::instance().playSFX("grenade.wav");
        Game::keyDown[SDLK_G] = false;

        Transform t;
        t.position = transform.position;
        t.rotation = transform.rotation;

        Vector2 dir(0, -1); // siempre hacia arriba

        if (grenades > 0)
        {
            isThrowing = true;
            SetAnimation("throw");
            grenades--;
            new Grenade(myScene, t, dir, this, false);
            grenadeActive = true;
        }
        else return;
    }
}

void Player::TakeDamage(int dmg)
{
    if (isDead)
        return;

    AudioManager::instance().playSFX("hit.wav");

    lives--;

    isDead = true;
    dying = true;
    deathTimer = 2.0f;
    SetAnimation("death");
    AudioManager::instance().playSFX("death.wav");
}

