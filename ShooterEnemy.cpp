#include "ShooterEnemy.h"
#include "Bullet.h"
#include "AudioManager.h"

ShooterEnemy::ShooterEnemy(Scene* scene, const Transform& t, Player* target)
    : Enemy(scene, t, target, "Shooter.png")
{
}

void ShooterEnemy::Update(float dt)
{
    // IA base
    Enemy::Update(dt);
}

void ShooterEnemy::ShootAtPlayer()
{
    if (!target) return;

    AudioManager::instance().playSFX("shoot.wav");

    // Direcci�n cardinal m�s cercana
    Vector2 realDir = target->transform.position - transform.position;
    realDir = realDir.normalize();

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

    // Crear transform del disparo
    Transform shot;
    shot.position = transform.position + bestDir * 30.f;
    shot.rotation = atan2(bestDir.y, bestDir.x) * 180.f / 3.14159f;

    // Crear bala enemiga
    new Bullet(myScene, shot, 400.f, true);
}