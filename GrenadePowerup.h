#pragma once
#include "CircleCollider.h"
#include "Sprite.h"

class Player;

class GrenadePowerup : public Sprite
{
public:
    GrenadePowerup(Scene* scene, const Vector2& pos);

    void OnTrigger(Actor* other) override;

private:
    Sprite* sprite = nullptr;
};
