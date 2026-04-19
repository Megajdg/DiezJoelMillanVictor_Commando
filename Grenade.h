#pragma once
#include "Sprite.h"
#include "Player.h"

// Forward declaration de Player
class Player;

/// <summary>
/// Gestiona como funcionan las granadas
/// </summary>
class Grenade : public AnimatedEntity
{
public:
    Grenade(Scene* scene, const Transform& t, Vector2 direction, Player* owner, bool fromEnemy = false);    // Constructor

    Player* owner = nullptr;                                                                                // Para liberar grenadeActive en el juego

    float timer = 0.f;                                                                                      // Reloj interno
    float fuseTime = 1.f;                                                                                   // Tiempo hasta explotar
    Vector2 velocity;                                                                                       // Velocidad actual, parabola
    float gravity = 900.f;                                                                                  // Gravedad vertical
    bool exploded = false;                                                                                  // Evita explotar dos veces
    bool fromEnemy = false;                                                                                 // Para saber a quien daña la explosion

    void Update(float dt) override;                                                                         // Fisica mas temporizador
    void Explode();                                                                                         // Crea explosion y se destruye
};