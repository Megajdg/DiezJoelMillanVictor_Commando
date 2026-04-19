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

// Funcion auxiliar que convierte un angulo en una animacion en movimiento, divide el circulo en 8
static std::string DirectionToAnimation(float angle)
{
    // Normalizamos el �ngulo a 0�360
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

// Convierte la animacion de idle correspondiente a la animacion de movimiento
static std::string IdleFromRun(const std::string& runAnim)
{
    if (runAnim == "run_front") return "idle_front";
    if (runAnim == "run_back") return "idle_back";

    // Laterales cualquiera vale
    if (runAnim == "run_left") return "idle_left";
    if (runAnim == "run_right") return "idle_right";

    // Diagonales simplificamos
    if (runAnim == "run_diag_front_left") return "idle_left";
    if (runAnim == "run_diag_front_right") return "idle_right";
    if (runAnim == "run_diag_back_left") return "idle_left";
    if (runAnim == "run_diag_back_right") return "idle_right";

    return "idle_front";
}

// Limita un valor entre un minimo y un maximo
template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// Cosntructor sin transform inical
Player::Player(Scene* myscene, std::string img_name, float side_size) : AnimatedEntity(myscene, img_name, Transform(), Vector2(side_size, side_size))
{
    Game::camera.target = this;             // La camara sigue al jugador
    weapon = new GunWeapon();               // Arma por defecto

    animationSet = LoadPlayerAnimations();  // Carga las animaciones del jugador
    SetAnimation("idle_front");             // Animacion inicial
}

// Constructor con transform inicial
Player::Player(Scene* myscene, std::string img_name, Transform transform, float side_size) : AnimatedEntity(myscene, img_name, transform, Vector2(side_size, side_size))
{

    Game::camera.target = this;             // La acamara sigue al jugador
    weapon = new GunWeapon();               // Arma por defecto

    animationSet = LoadPlayerAnimations();  // Carga las animaciones del jugador
    SetAnimation("idle_front");             // Animacion inicial

}

void Player::Update(float deltaTime)
{
    AnimatedEntity::Update(deltaTime);  // Avanza animaciones
    UpdateMovement(deltaTime);          // Movimiento + input

    // Limite del mapa
    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    // Limitar X dentro del mapa
    transform.position.x = Clamp(transform.position.x, mapLeft, mapRight);

    // Limitar Y dentro del mapa
    transform.position.y = Clamp(transform.position.y, mapTop, mapBottom);

    // Convertir posici�n del jugador a pantalla
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
    // Si el jugador esta en proceso de morir, no puede moverse ni disparar, esperamos a que termine la animación de muerte.
    if (dying)
    {
        deathTimer -= deltaTime;

        // Al llegar a 0 decimos si respawnear o termina la partida
        if (deathTimer <= 0.f)
        {
            if (lives > 0)
                Game::OnPlayerDeath(this);
            else
                Game::OnGameOver();
        }

        return;
    }

    // Direccion del movimiento segun teclas
    Vector2 dir(0, 0);

    bool up = Game::keyDown[SDLK_W] || Game::keyDown[SDLK_UP];
    bool down = Game::keyDown[SDLK_S] || Game::keyDown[SDLK_DOWN];
    bool left = Game::keyDown[SDLK_A] || Game::keyDown[SDLK_LEFT];
    bool right = Game::keyDown[SDLK_D] || Game::keyDown[SDLK_RIGHT];

    // Direccion segun teclas pulsadas
    if (up)    dir.y -= 1;
    if (down)  dir.y += 1;
    if (left)  dir.x -= 1;
    if (right) dir.x += 1;

    // Si esta lanzando granada, esperar a que termine la animacion
    if (isThrowing)
    {
        Animation* a = currentAnimation;

        // Cuando la animacion termina, volvemos a idle correspondiente
        if (a->finished)
        {
            isThrowing = false;
            SetAnimation(IdleFromRun(lastMoveAnim));
        }
        return;
    }

    // Movimiento normal, evitamos que en diagonal vaya mas rapido
    if (dir.x != 0 || dir.y != 0)
    {
        dir = dir.normalize();

        float speed = 300.f;
        transform.position += dir * speed * deltaTime;

        // Rotacion del sprite segun direccion
        float angle = atan2(dir.y, dir.x) * 180.f / 3.14159f;
        angle += 90.f;
        transform.rotation = angle;

        // Animacion correspondiente
        std::string anim = DirectionToAnimation(angle);
        SetAnimation(anim);
        lastMoveAnim = anim;
    }
    else
    {
        // Si no hay animacion, usamos la animacion idle correspondiente
        SetAnimation(IdleFromRun(lastMoveAnim));
    }

    // Disparo
    if (Game::keyDown[SDLK_SPACE])
    {
        AudioManager::instance().PlaySFX("shoot.wav");
        Game::keyDown[SDLK_SPACE] = false;              // Evitamos disparo continuo por frame

        Transform shot;
        shot.position = transform.position;

        // La rotacion de disparo se basa en la rotacion del jugador
        shot.rotation = transform.rotation - 90.f;

        weapon->Shoot(myScene, shot);
    }

    // Lanzar granada con G
    if (Game::keyDown[SDLK_G] && !grenadeActive)
    {
        AudioManager::instance().PlaySFX("grenade.wav");
        Game::keyDown[SDLK_G] = false;

        Transform t;
        t.position = transform.position;
        t.rotation = transform.rotation;

        Vector2 dir(0, -1);                 // Siempre hacia arriba, no siguen la rotacion del jugador

        if (grenades > 0)
        {
            isThrowing = true;              // Bloquea el movimiento hasta terminar la animacion
            SetAnimation("throw");          // Animacion de lanzamiento
            grenades--;                     // Consume granada

            // Crea una granada en escena y evita lanzar otra hasta que explote
            new Grenade(myScene, t, dir, this, false);
            grenadeActive = true;
        }
        else return;
    }
}

void Player::TakeDamage(int dmg)
{
    // Si el jugador ya esta marcado como muerto ignora el resto, evitamos reproducir animaciones o sonidos repetidos
    if (isDead)
        return;

    // Sonido de impacto al recibir daño
    AudioManager::instance().PlaySFX("hit.wav");

    // Reducir vidas
    lives--;

    // Marca al jugador como muerto
    isDead = true;

    // Activa muriendo y se espera un tiempo antes de respawnear o terminar la partida
    dying = true;
    deathTimer = 2.0f;

    // Cambia de animacion
    SetAnimation("death");

    // Sonido de muerte
    AudioManager::instance().PlaySFX("death.wav");
}