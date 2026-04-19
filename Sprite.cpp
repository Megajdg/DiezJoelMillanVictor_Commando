#include "Sprite.h"
#include "Scene.h"
#include "GraphicsInterface.h"
#include "AnimatedEntity.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "SDL3/SDL_render.h"

// Constructor basico que crea un sprite con imagen y tamaño y coloca el transform en 0 por defecto
Sprite::Sprite(Scene* myscene, std::string img_name, Vector2 size) : Actor(myscene)
{
    image_name = img_name;
    this->size = size;
}

// Cosntrtuctor alternativo que permite inicializar el transform desde fuera
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

// Dibuja el sprite por pantalla, si el sprite es un AnimatedEntity, usa su frame actual del spritesheet
void Sprite::Render()
{
    // Rectangulo de origen dentro del spritesheet
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

    // Dibujar usando GraphicsInterface, si hay srcPtr dibuja un frame en concreto de la animacion
    if (srcPtr)
        myScene->GI->DrawSprite(image_name, transform, size, srcPtr);
    else
        myScene->GI->DrawSprite(image_name, transform, size);
}