#pragma once
#include "Sprite.h"
#include "AnimationSet.h"
#include "Animation.h"
#include <string>

/// <summary>
/// Extiende Sprite, añadiendo un conjunto de animaciones, la animacion activa y la logica para cambiar y actualizar animaciones
/// </summary>
class AnimatedEntity : public Sprite
{
public:
    AnimationSet animationSet;                                                          // Contiene todas las animaciones de la entidad
    Animation* currentAnimation = nullptr;                                              // Puntero a la animacion actual

    AnimatedEntity(Scene* scene, std::string img, Transform t, Vector2 size);           // Constructor con transform completo
    AnimatedEntity(Scene* scene, std::string img, Vector2 size);                        // Constructor sin posicion explicita
    AnimatedEntity(Scene* scene, std::string img, const Vector2& pos, Vector2 size);    // Constructor con posicion y tamaño

    virtual void SetAnimation(const std::string& name);                                 // Cambia la animacion activa
    virtual void Update(float dt) override;                                             // Avanza la animacion
};
