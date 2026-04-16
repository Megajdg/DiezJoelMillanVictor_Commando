#pragma once
#include "Sprite.h"
#include "Player.h"

class Player;

class Grenade : public Sprite
{
public:
    Grenade(Scene* scene, const Transform& t, Vector2 direction, Player* owner, bool fromEnemy = false);

    Player* owner = nullptr;

    float timer = 0.f;
    float fuseTime = 0.6f; // tiempo hasta explotar
    Vector2 velocity;
    float gravity = 900.f;
    bool exploded = false;
    bool fromEnemy = false;

    void Update(float dt) override;
    void Explode();
};
