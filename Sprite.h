#pragma once
#include "Actor.h"
#include "Vector2.h"
#include "SDL3/SDL.h"
#include <iostream>
#include <map>
#include "Animation.h"

class Sprite :public Actor
{
protected:
	std::string image_name;
	Vector2 size;

	std::map<std::string, Animation*> animations;
	Animation* currentAnimation = nullptr;
	SDL_FRect srcRect;

public:
	Sprite(Scene *myscene, std::string img_name, Vector2 size);
	Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size);

	virtual void Update(float deltaTime);
	virtual void Render();

	void SetAnimation(const std::string& name);
	void UpdateAnimation(float deltaTime);
};

