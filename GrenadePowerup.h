#pragma once
#include "CircleCollider.h"
#include "AnimatedEntity.h"


class Player;

class GrenadePowerup : public AnimatedEntity
{
public:
    GrenadePowerup(Scene* scene, const Vector2& pos);

    void Update(float dt) override;
    void OnTrigger(Actor* other) override;

};
