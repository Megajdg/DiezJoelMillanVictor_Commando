#pragma once
#include "Actor.h"

class EnemySpawner : public Actor
{
public:
    float timer = 0.f;
    float spawnRate = 2.f;

    EnemySpawner(Scene* scene);

    void Update(float dt) override;
};
