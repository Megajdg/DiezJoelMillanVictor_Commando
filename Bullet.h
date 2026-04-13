#pragma once
#include "Sprite.h"

class Bullet : public Sprite
{
public:
    Bullet(Scene* scene, const Transform& t, float speed);

    void Update(float dt) override;

private:
    float speed;
};