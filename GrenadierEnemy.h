#pragma once
#include "Enemy.h"
#include "Player.h"

/// <summary>
/// Enemigo que dispara granadas directamente hacia el jugador, redefine el comportamiento de disparo
/// </summary>
class GrenadierEnemy : public Enemy
{
public:
    GrenadierEnemy(Scene* scene, const Transform& t, Player* target);   // Constructor que recibe la escena, el transform inicial y el objetivo, en este caso el player

    float grenadeCooldown = 3.0f;                                       // Tiempo entre lanzamientos
    float grenadeTimer = 0.f;                                           // Temporizador interno

    void Update(float dt) override;                                     // Bloquea IA si esta lanzando
    void ShootAtPlayer() override;                                      // Siempre lanza granada
    void ThrowGrenade();                                                // Logica del lanzamiento
};