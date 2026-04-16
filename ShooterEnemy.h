#pragma once
#include "Enemy.h"

class ShooterEnemy : public Enemy
{
public:
    ShooterEnemy(Scene* scene, const Transform& t, Player* target);

    float shootCooldown = 1.5f;
    float shootTimer = 0.f;

    void Update(float dt) override;
    void ShootAtPlayer() override;
};
