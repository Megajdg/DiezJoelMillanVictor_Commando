#include "GunWeapon.h"
#include "Bullet.h"
#include "Scene.h"

void GunWeapon::Shoot(Scene* scene, const Transform& origin)
{
    // Crear una bala que sale desde el transform del arma
    Bullet* b = new Bullet(scene, origin, 800.f, false);
}