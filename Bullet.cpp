#include "Bullet.h"
#include "Scene.h"
#include "Game.h"
#include "CircleCollider.h"
#include "MyPhysics.h"
#include "Enemy.h"
#include "Player.h"
#include "AnimationProjectile.h"

Bullet::Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy) : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100)), speed(speed), fromEnemy(fromEnemy)
{
    // Registramos el actor en la escena con posicion, velocidad y si viene del enemigo o no
    scene->AddActor(this);

    // Cargamos las animaciones del spritesheet de proyectiles y activamos la animacion de bullet
    animationSet = LoadProjectileAnimations();
    SetAnimation("bullet");
    
    // Añadimos un collider con trigger de radio 10
    CircleCollider* col = new CircleCollider();
    col->radius = 10;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Bullet::Update(float dt)
{
    AnimatedEntity::Update(dt);

    // Si esta en animación de impacto, esperamos a que termine antes de destruirla
    if (hit)
    {
        if (currentAnimation->finished)
            myScene->DestroyActor(this);
        return;
    }

    // De grados a radianes
    float rad = transform.rotation * 3.14159f / 180.f;

    // Hacemos que avance en la direccion del ángulo del transform
    Vector2 oldPos = transform.position;

    transform.position.x += cos(rad) * speed * dt;
    transform.position.y += sin(rad) * speed * dt;

    // Calcula la distancia recorrida en este frame
    traveled += (transform.position - oldPos).Module();

    // Si supera la distancia maxima, hacemos que impacte y activamos la animacion de impacto
    if (traveled >= maxDistance)
    {
        hit = true;
        SetAnimation("bullet_hit");
    }
}

void Bullet::OnTrigger(Actor* other)
{
    // Si la bala viene del enemigo, comprobamos si esta chocando con el player, y si es asi paramos la bala y mostramos la animacion de impacto
    if (fromEnemy)
    {
        Player* p = dynamic_cast<Player*>(other);
        if (p)
        {
            p->TakeDamage(1);
            hit = true;
            speed = 0.f;
            SetAnimation("bullet_hit");
        }
    }
    // Si la bala viene del player, comprobamos si esta chocando con un enemigo, y si es asi paramos la bala y mostramos la animacion de impacto
    else
    {
        Enemy* e = dynamic_cast<Enemy*>(other);
        if (e)
        {
            e->TakeDamage(1);
            hit = true;
            speed = 0.f;
            SetAnimation("bullet_hit");
        }
    }
}
