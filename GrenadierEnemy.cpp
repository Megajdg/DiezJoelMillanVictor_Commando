#include "GrenadierEnemy.h"
#include "Grenade.h"
#include "Scene.h"
#include "AudioManager.h"

GrenadierEnemy::GrenadierEnemy(Scene* scene, const Transform& t, Player* target) : Enemy(scene, t, target, "Grenadier.png")
{
}

void GrenadierEnemy::Update(float dt)
{
    // Si esta en animación de throw, esperar a que termine antes de usar IA
    if (currentAnimation && currentAnimation->name == "throw" && !currentAnimation->finished)
    {
        AnimatedEntity::Update(dt);
        return;
    }

    Enemy::Update(dt);
}

void GrenadierEnemy::ShootAtPlayer()
{
    // Este enemigo siempre lanza granadas
    ThrowGrenade();
}

void GrenadierEnemy::ThrowGrenade()
{
    if (!target) return;

    // Activar animacion de lanzamiento
    SetAnimation("throw");

    // Sonido granada
    AudioManager::instance().PlaySFX("grenade.wav");

    // Direccion real hacia el jugador
    Vector2 realDir = target->transform.position - transform.position;
    realDir = realDir.normalize();

    // 8 direcciones cardinales
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    // Elegir la direccion mas cercana
    float bestDot = -9999.f;
    Vector2 bestDir;

    // Elegir la direccion cardinal mas parecida a la real
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