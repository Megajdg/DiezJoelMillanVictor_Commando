#pragma once
#include "Weapon.h"

/// <summary>
/// Arma basica que dispara una bala recta desde el origen
/// </summary>
class GunWeapon : public Weapon
{
public:
    void Shoot(Scene* scene, const Transform& origin) override; // Crear bala
};