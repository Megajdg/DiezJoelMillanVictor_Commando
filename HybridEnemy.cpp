#include "HybridEnemy.h"
#include "Bullet.h"
#include "Grenade.h"
#include <cstdlib>
#include "AudioManager.h"

HybridEnemy::HybridEnemy(Scene* scene, const Transform& t, Player* target) : Enemy(scene, t, target, "Hybrid.png")
{
}

void HybridEnemy::Update(float dt)
{
    // Si esta en animación de throw, esperar a que termine antes de usar IA
    if (currentAnimation && currentAnimation->name == "throw" && !currentAnimation->finished)
    {
        AnimatedEntity::Update(dt);
        return;
    }

    Enemy::Update(dt);
}

void HybridEnemy::ShootAtPlayer()
{
    // Posibilidad del 50% que dispare una bala o una granada
    int r = rand() % 2;
    if (r == 0)
        ShootBullet();
    else
        ThrowGrenade();
}

void HybridEnemy::ShootBullet()
{
    // Sonido del disparo
    AudioManager::instance().PlaySFX("shoot.wav");

    // Direccion hacia el jugador
    Vector2 realDir = (target->transform.position - transform.position).normalize();

    // 8 direcciones cardinales
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float bestDot = -9999.f;
    Vector2 bestDir;

    // Elegir direccion cardinal más cercana
    for (int i = 0; i < 8; i++)
    {
        Vector2 d = dirs[i].normalize();
        float dot = realDir.x * d.x + realDir.y * d.y;

        if (dot > bestDot)
        {
            bestDot = dot;
            bestDir = d;
        }
    }

    // Crear transform del disparo desplazado para que apareza desde dentro del player
    Transform shot;
    shot.position = transform.position + bestDir * 30.f;

    // Rotacion en grados segun la direccion elegida
    shot.rotation = atan2(bestDir.y, bestDir.x) * 180.f / 3.14159f;

    // Crear bala enemiga
    new Bullet(myScene, shot, 400.f, true);
}

void HybridEnemy::ThrowGrenade()
{
    // Sonido de la granada
    AudioManager::instance().PlaySFX("grenade.wav");

    // Direccion real hacia el jugador
    Vector2 realDir = (target->transform.position - transform.position).normalize();

    // Activar animacion de lanzamiento
    SetAnimation("throw");

    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float bestDot = -9999.f;
    Vector2 bestDir;

    // Elegir direccion cardinal más cercana
    for (int i = 0; i < 8; i++)
    {
        Vector2 d = dirs[i].normalize();
        float dot = realDir.x * d.x + realDir.y * d.y;

        if (dot > bestDot)
        {
            bestDot = dot;
            bestDir = d;
        }
    }

    // Crear transform de la granada
    Transform t;
    t.position = transform.position + bestDir * 30.f;
    t.rotation = atan2(bestDir.y, bestDir.x) * 180.f / 3.14159f;

    // Crear granada enemiga
    new Grenade(myScene, t, bestDir, nullptr, true);
}