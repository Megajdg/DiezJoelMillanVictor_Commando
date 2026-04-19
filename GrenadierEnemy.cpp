#include "GrenadierEnemy.h"
#include "Grenade.h"
#include "Scene.h"
#include "AudioManager.h"

GrenadierEnemy::GrenadierEnemy(Scene* scene, const Transform& t, Player* target)
    : Enemy(scene, t, target, "Grenadier.png")
{
}

void GrenadierEnemy::Update(float dt)
{
    // IA base
    Enemy::Update(dt);
}

void GrenadierEnemy::ShootAtPlayer()
{
    ThrowGrenade();
}

void GrenadierEnemy::ThrowGrenade()
{
    if (!target) return;
    SetAnimation("throw");

    AudioManager::instance().playSFX("grenade.wav");

    // Direcci�n real hacia el jugador
    Vector2 realDir = target->transform.position - transform.position;
    realDir = realDir.normalize();

    // 8 direcciones cardinales
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    // Elegir la direcci�n m�s cercana
    float bestDot = -9999.f;
    Vector2 bestDir;

    for (int i = 0; i < 8; i++)
    {
        Vector2 d = dirs[i].normalize();
        float dot = realDir.x * d.x + realDir.y * d.y; // producto punto

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

    // Lanzar granada
    new Grenade(myScene, t, bestDir, nullptr, true);
}

