#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GunWeapon.h"

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
}

void Player::UpdateMovement(float deltaTime)
{
	Vector2 direction;

	if (Game::keyDown[SDLK_A])
		transform.rotation -= ang_speed * deltaTime;
	if (Game::keyDown[SDLK_D])
		transform.rotation += ang_speed * deltaTime;

	direction.y = -std::cos(transform.rotation *3.14159/180);
	direction.x = std::sin(transform.rotation *3.14159/180);
	if (Game::keyDown[SDLK_W])
		transform.position += direction * linear_speed * deltaTime;
	if (Game::keyDown[SDLK_S])
		transform.position -= direction * linear_speed * deltaTime;

	if (Game::keyDown[SDLK_SPACE])
	{
		Game::keyDown[SDLK_SPACE] = false;

		Transform shot;
		shot.position = transform.position + Vector2(cos(transform.rotation * 0.01745f),
			sin(transform.rotation * 0.01745f)) * 50.f;

		shot.rotation = transform.rotation;

		weapon->Shoot(myScene, shot);
	}
}
