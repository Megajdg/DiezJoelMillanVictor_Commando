#pragma once
#include "Sprite.h"
#include "Weapon.h"

class Player : public Sprite
{
public:
	Player(Scene* myscene, std::string img_name, float side_size);
	Player(Scene* myscene, std::string img_name, Transform transform, float side_size);

	virtual void Update(float deltaTime);

private:

	void UpdateMovement(float deltaTime);
	std::string lastMoveAnim = "run_front";
	float linear_speed = 300.f;
	float ang_speed = 100.f;
	Weapon* weapon = nullptr;
	bool isThrowing = false;
	bool isDead = false;
};

