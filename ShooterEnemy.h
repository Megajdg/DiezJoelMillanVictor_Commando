#pragma once
#include "Enemy.h"

/// <summary>
/// Enemigo que dispara balas directamente hacia el jugador y redefine el comportamiento de disparo
/// </summary>
class ShooterEnemy : public Enemy
{
public:
    ShooterEnemy(Scene* scene, const Transform& t, Player* target); // Constructor que recibe la escena, el transform inicial y el objetivo, en este caso el player

    float shootCooldown = 1.5f;                                     // Tiempo entre disparo
    float shootTimer = 0.f;                                         // Temporizador interno

    void Update(float dt) override;                                 // Gestiona logica
    void ShootAtPlayer() override;                                  // Dispara balas al jugador
};