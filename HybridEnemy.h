#pragma once
#include "Enemy.h"

class HybridEnemy : public Enemy
{
public:
    HybridEnemy(Scene* scene, const Transform& t, Player* target);

    void Update(float dt) override;

private:
    float attackTimer = 0.f;
    float attackCooldown = 2.0f; // cada 2 segundos

    void ShootAtPlayer() override;
    void DoRandomAttack();
    void ShootBullet();
    void ThrowGrenade();
};