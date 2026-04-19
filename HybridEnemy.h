#pragma once
#include "Enemy.h"

/// <summary>
/// Enemigo que dispara balas y granadas directamente hacia el jugador, redefine el comportamiento de disparo
/// </summary>
class HybridEnemy : public Enemy
{
public:
    HybridEnemy(Scene* scene, const Transform& t, Player* target);  // Constructor que recibe la escena, el transform inicial y el objetivo, en este caso el player

    void Update(float dt) override;                                 // Gestiona logica

private:
    float attackTimer = 0.f;                                        // Tiempo entre disparos
    float attackCooldown = 2.0f;                                    // Temporizador interno

    void ShootAtPlayer() override;                                  // Decide si bala o granada
    void ShootBullet();                                             // Disparo en 8 direcciones
    void ThrowGrenade();                                            // Lanzamiento con animacion
};