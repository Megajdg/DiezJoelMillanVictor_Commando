#include "ShooterEnemy.h"
#include "Bullet.h"
#include "AudioManager.h"

// Constructor que llama al constructor base Enemy con la textura de Sooter.png
ShooterEnemy::ShooterEnemy(Scene* scene, const Transform& t, Player* target)
    : Enemy(scene, t, target, "Shooter.png")
{
}

void ShooterEnemy::Update(float dt)
{
    // usa la IA general del enemigo
    Enemy::Update(dt);
}

void ShooterEnemy::ShootAtPlayer()
{
    if (!target) return; // Si no hay jugador, no dispara

    // Sonido del disparo
    AudioManager::instance().PlaySFX("shoot.wav");

    // Direccion real hacia el jugador.    
    Vector2 realDir = target->transform.position - transform.position;
    realDir = realDir.normalize();

    // Direcciones cardinales y diagonales para elegir la mas cercana.
    static Vector2 dirs[8] = {
        {  1,  0 }, {  1,  1 }, {  0,  1 }, { -1,  1 },
        { -1,  0 }, { -1, -1 }, {  0, -1 }, {  1, -1 }
    };

    float bestDot = -9999.f;
    Vector2 bestDir;

    // Elegir la direccion cardinal mas parecida a la direccion real.
    // Esto hace que el enemigo dispare en 8 direcciones discretas.
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
    new Bullet(myScene, shot, 300.f, true);
}