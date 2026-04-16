#pragma once
#include "Sprite.h"

class Bullet : public Sprite
{
public:
    Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy = false);

    void Update(float dt) override;

    void OnTrigger(Actor* otherActor) override;

    bool fromEnemy;

    float maxDistance = 400.f;
    float traveled = 0.f;

private:
    float speed;
};