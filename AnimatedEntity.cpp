#include "AnimatedEntity.h"

AnimatedEntity::AnimatedEntity(Scene* scene, std::string img, Transform t, Vector2 size) : Sprite(scene, img, t, size)
{
}

AnimatedEntity::AnimatedEntity(Scene* scene, std::string img, Vector2 size) : Sprite(scene, img, size)
{
}

// Construimos la entidad con una posicion definida
AnimatedEntity::AnimatedEntity(Scene* scene, std::string img, const Vector2& pos, Vector2 size) : Sprite(scene, img, Transform(), size)
{
    transform.position = pos;
}

void AnimatedEntity::SetAnimation(const std::string& name)
{
    // Buscamos la animacion por nombre, si no existe abortamos
    Animation* a = animationSet.Get(name);
    if (!a) return;

    // Si es distinta a la actual, la sustituimos y la reseteamos
    if (a != currentAnimation)
    {
        currentAnimation = a;
        currentAnimation->Reset();
    }
}

void AnimatedEntity::Update(float dt)
{
    Sprite::Update(dt);

    // Si hay animacion activa, actualizamos el frame segun el tiempo de ejecucion
    if (currentAnimation)
        currentAnimation->Update(dt);
}