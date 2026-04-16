#include "Sprite.h"
#include "Scene.h"
#include "GraphicsInterface.h"
#include "AnimatedEntity.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_render.h"


Sprite::Sprite(Scene* myscene, std::string img_name, Vector2 size)
    : Actor(myscene)
{
    image_name = img_name;
    this->size = size;
}

Sprite::Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size)
    : Actor(myscene)
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
    SDL_FRect srcRect;
    SDL_FRect* srcPtr = nullptr;

    // Si es AnimatedEntity, usar su frame actual
    AnimatedEntity* anim = dynamic_cast<AnimatedEntity*>(this);
    if (anim && anim->currentAnimation)
    {
        Frame f = anim->currentAnimation->GetCurrentFrame();
        srcRect.x = f.x;
        srcRect.y = f.y;
        srcRect.w = f.w;
        srcRect.h = f.h;
        srcPtr = &srcRect;
    }

    // Dibujar usando GraphicsInterface
    if (srcPtr)
        myScene->GI->DrawSprite(image_name, transform, size, srcPtr);
    else
        myScene->GI->DrawSprite(image_name, transform, size);
}
