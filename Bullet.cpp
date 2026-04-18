#include "Bullet.h"
#include "Scene.h"
#include "Game.h"
#include "CircleCollider.h"
#include "MyPhysics.h"
#include "Enemy.h"
#include "Player.h"
#include "AnimationProjectile.h"

Bullet::Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy)
    : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100)), speed(speed), fromEnemy(fromEnemy)
{
    scene->AddActor(this);

    animationSet = LoadProjectileAnimations();
    SetAnimation("bullet");
    
    CircleCollider* col = new CircleCollider();
    col->radius = 10;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Bullet::Update(float dt)
{

    AnimatedEntity::Update(dt);

    // Si está en animación de impacto, esperar a que termine
    if (hit)
    {
        if (currentAnimation->finished)
            myScene->DestroyActor(this);
        return;
    }

    // Avanza en la dirección del ángulo del transform
    float rad = transform.rotation * 3.14159f / 180.f;

    Vector2 oldPos = transform.position;

    transform.position.x += cos(rad) * speed * dt;
    transform.position.y += sin(rad) * speed * dt;

    // Distancia recorrida en este frame
    traveled += (transform.position - oldPos).Module();

    // Si supera la distancia máxima, destruir
    if (traveled >= maxDistance)
    {
        hit = true;
        SetAnimation("bullet_hit");
    }
}

void Bullet::OnTrigger(Actor* other)
{
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
