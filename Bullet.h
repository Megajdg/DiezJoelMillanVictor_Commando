#pragma once
#include "AnimatedEntity.h"

/// <summary>
/// Gestiona como funcionan las balas
/// </summary>
class Bullet : public AnimatedEntity
{
public:
    Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy);  // Constructor

    float speed;                                                            // Velocidad lineal de la bala
    bool fromEnemy;                                                         // Determina a quien puede dañar

    float traveled = 0.f;                                                   // Distancia recorrida
    float maxDistance = 400.f;                                              // Limite antes de destruirse

    bool hit = false;                                                       // Indica si ya ha impactado

    void Update(float dt) override;                                         // Gestiona movimiento e impacto
    void OnTrigger(Actor* other) override;                                  // Aplica daño segun origen
};