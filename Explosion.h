#pragma once
#include "AnimatedEntity.h"

class Explosion : public AnimatedEntity
{
public:
    float timer = 0.f;
    float duration = 0.2f;
    float radius = 50.f;
    bool dying = false;
    bool fromEnemy = false;

    Explosion(Scene* scene, const Transform& t, bool fromEnemy);

    void Update(float dt) override;
    void OnTrigger(Actor* otherActor) override;
};
