#pragma once
#include "Actor.h"

class Weapon
{
public:
    virtual ~Weapon() {}
    virtual void Shoot(class Scene* scene, const class Transform& origin) = 0;
};