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
	UpdateAnimation(deltaTime);
}

void Sprite::Render()
{
	if (currentAnimation)
		myScene->GI->DrawSprite(image_name, transform, size, &srcRect);
	else
		myScene->GI->DrawSprite(image_name, transform, size);
}

void Sprite::SetAnimation(const std::string& name)
{
    auto it = animations.find(name);
    if (it == animations.end())
        return;

    if (currentAnimation != it->second)
    {
        currentAnimation = it->second;
        currentAnimation->Reset();
    }
}

void Sprite::UpdateAnimation(float deltaTime)
{
    if (!currentAnimation)
        return;

    currentAnimation->Update(deltaTime);

    // Copiamos el frame actual al srcRect
    srcRect = currentAnimation->GetCurrentFrame();
}

SDL_FRect MakeFrame(int col, int row, float startX, float startY, float frameW, float frameH, float sepX, float sepY)
{
    SDL_FRect r;
    r.x = startX + col * (frameW + sepX);
    r.y = startY + row * (frameH + sepY);
    r.w = frameW;
    r.h = frameH;
    return r;
}