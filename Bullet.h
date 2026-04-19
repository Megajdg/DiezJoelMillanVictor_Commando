#pragma once
#include "AnimatedEntity.h"

class Bullet : public AnimatedEntity
{
public:
    Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy);

    float speed;
    bool fromEnemy;

    float traveled = 0.f;
    float maxDistance = 400.f;

    bool hit = false;

    void Update(float dt) override;
    void OnTrigger(Actor* other) override;
};
