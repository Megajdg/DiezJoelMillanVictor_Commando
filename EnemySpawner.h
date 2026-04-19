#pragma once
#include "Actor.h"

/// <summary>
/// Define el controlador de spawns de enemigos
/// </summary>
class EnemySpawner : public Actor
{
public:
    float timer = 0.f;                  // Reloj interno
    float spawnRate = 1.5f;             // Tiempo entre spawns

    EnemySpawner(Scene* scene);         // Constructor

    void Update(float dt) override;     // Contiene la logica de spawn
};