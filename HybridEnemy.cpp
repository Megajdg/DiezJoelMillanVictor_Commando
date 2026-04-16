#include "HybridEnemy.h"
#include "Bullet.h"
#include "Grenade.h"
#include <cstdlib> // rand()

HybridEnemy::HybridEnemy(Scene* scene, const Transform& t, Player* target) : Enemy(scene, t, target)
{
    image_name = "Joe.png"; // o el sprite que quieras
}

void HybridEnemy::Update(float dt)
{
    // IA base
    Enemy::Update(dt);
}

void HybridEnemy::ShootAtPlayer()
{
    int r = rand() % 2;
    if (r == 0)
        ShootBullet();
    else
        ThrowGrenade();
}

void HybridEnemy::ShootBullet()
{
    // Dirección hacia el jugador
    Vector2 realDir = (target->transform.position - transform.position).normalize();

    // 8 direcciones cardinales
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float bestDot = -9999.f;
    Vector2 bestDir;

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

    Transform shot;
    shot.position = transform.position + bestDir * 30.f;
    shot.rotation = atan2(bestDir.y, bestDir.x) * 180.f / 3.14159f;

    new Bullet(myScene, shot, 400.f, true);
}

void HybridEnemy::ThrowGrenade()
{
    Vector2 realDir = (target->transform.position - transform.position).normalize();

    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float bestDot = -9999.f;
    Vector2 bestDir;

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

    Transform t;
    t.position = transform.position + bestDir * 30.f;
    t.rotation = atan2(bestDir.y, bestDir.x) * 180.f / 3.14159f;

    new Grenade(myScene, t, bestDir, nullptr, true);
}
