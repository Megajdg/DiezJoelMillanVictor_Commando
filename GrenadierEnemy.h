#pragma once
#include "Enemy.h"
#include "Player.h"

class GrenadierEnemy : public Enemy
{
public:
    GrenadierEnemy(Scene* scene, const Transform& t, Player* target);

    float grenadeCooldown = 3.0f;
    float grenadeTimer = 0.f;

    void Update(float dt) override;
    void ShootAtPlayer() override;
    void ThrowGrenade();
};
