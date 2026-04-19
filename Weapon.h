#pragma once
#include "Actor.h"

/// <summary>
/// Clase para las armas del juego solo generan proyectiles
/// </summary>
class Weapon
{
public:
    virtual ~Weapon() {}

    virtual void Shoot(class Scene* scene, const class Transform& origin) = 0;
};