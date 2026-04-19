#pragma once
#include "CircleCollider.h"
#include "AnimatedEntity.h"

// Forward declaration de Player
class Player;

/// <summary>
/// Define un powerup que da granadas al jugador al tocarlo
/// </summary>
class GrenadePowerup : public AnimatedEntity
{
public:
    GrenadePowerup(Scene* scene, const Vector2& pos);   // Crea powerup animado

    void Update(float dt) override;                     // Avanza animacion
    void OnTrigger(Actor* other) override;              // Da granadas y se destruye
};
