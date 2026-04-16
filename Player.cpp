#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GunWeapon.h"
#include "Grenade.h"
#include "GameScene.h"
#include "Parameters.h"

template<typename T>
T Clamp(const T& value, const T& minVal, const T& maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

Player::Player(Scene* myscene, std::string img_name, float side_size) : Sprite(myscene, img_name, Vector2(side_size, side_size))
{
	Game::camera.target = this;
	weapon = new GunWeapon();

}

Player::Player(Scene* myscene, std::string img_name, Transform transform, float side_size) : Sprite(myscene, img_name, transform, Vector2(side_size, side_size))
{
	Game::camera.target = this;
	weapon = new GunWeapon();
}

void Player::Update(float deltaTime)
{
    UpdateMovement(deltaTime);

    float mapLeft = -1272 * 0.5f + 50;
    float mapRight = 1272 * 0.5f - 50;
    float mapTop = -6232 + 50;
    float mapBottom = 0;

    // Limitar X
    transform.position.x = Clamp(transform.position.x, mapLeft, mapRight);

    float hudHeight = 200;
    float limitBottom = Game::camera.position.y + Parameters::screenHeight * 0.5f - hudHeight - 50;

    // Limitar Y dentro del mapa
    transform.position.y = Clamp(transform.position.y, mapTop, mapBottom);

    // Límite inferior real
    if (transform.position.y > limitBottom)
        transform.position.y = limitBottom;
}

void Player::UpdateMovement(float deltaTime)
{
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

    // Disparo
    if (Game::keyDown[SDLK_SPACE])
    {
        Game::keyDown[SDLK_SPACE] = false;

        Transform shot;
        shot.position = transform.position + Vector2(cos(transform.rotation * 0.01745f),
            sin(transform.rotation * 0.01745f)) * 50.f;

        shot.rotation = transform.rotation;

        weapon->Shoot(myScene, shot);
    }

    if (Game::keyDown[SDLK_G] && !grenadeActive)
    {
        Game::keyDown[SDLK_G] = false;

        Transform t;
        t.position = transform.position;
        t.rotation = transform.rotation;

        Vector2 dir(0, -1); // siempre hacia arriba

        if (grenades > 0)
        {
            grenades--;
            new Grenade(myScene, t, dir, this, false);
            grenadeActive = true;
        }
        else return;
    }
}

void Player::TakeDamage(int dmg)
{
    lives--;

    if (lives > 0)
    {
        Game::OnPlayerDeath(this);
    }
    else
    {
        Game::OnGameOver();
    }
}

