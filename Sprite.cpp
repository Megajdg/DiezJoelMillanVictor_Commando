#include "Sprite.h"
#include "Scene.h"
#include "GraphicsInterface.h"

Sprite::Sprite(Scene* myscene, std::string img_name, Vector2 size):Actor(myscene)
{
	image_name = img_name;
	this->size = size;
}

Sprite::Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size):Actor(myscene)
{
	image_name = img_name;
	this->transform = transform;
	this->size = size;
}

void Sprite::Update(float deltaTime)
{
}

void Sprite::Render()
{
	myScene->GI->DrawSprite(image_name, transform, size);
}
