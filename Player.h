#pragma once
#include "Sprite.h"
#include "Weapon.h"

class Player : public Sprite
{
public:
	Player(Scene* myscene, std::string img_name, float side_size);
	Player(Scene* myscene, std::string img_name, Transform transform, float side_size);
	
	int lives = 5;
	int grenades = 5;

	virtual void Update(float deltaTime);
	bool grenadeActive = false;
	void TakeDamage(int dmg);

private:

	void UpdateMovement(float deltaTime);

	float linear_speed = 300.f;
	float ang_speed = 100.f;
	Weapon* weapon = nullptr;
};

