#pragma once
#include "AnimatedEntity.h"

/// <summary>
/// Gestiona como funcionan las explosiones
/// </summary>
class Explosion : public AnimatedEntity
{
public:
    float radius = 50.f;                                            // Radio del collider
    bool fromEnemy = false;                                         // Determina si viene del enemigo o no

    Explosion(Scene* scene, const Transform& t, bool fromEnemy);    // Constructor

    void Update(float dt) override;                                 // Gestiona animacion
    void OnTrigger(Actor* otherActor) override;                     // Aplica daño segun origen
};
