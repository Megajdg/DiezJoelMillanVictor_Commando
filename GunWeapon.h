#pragma once
#include "Weapon.h"

class GunWeapon : public Weapon
{
public:
    void Shoot(Scene* scene, const Transform& origin) override;
};
