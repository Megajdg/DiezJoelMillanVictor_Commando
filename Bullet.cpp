#include "Bullet.h"
#include "Scene.h"
#include "Game.h"
#include "CircleCollider.h"
#include "MyPhysics.h"
#include "Enemy.h"

Bullet::Bullet(Scene* scene, const Transform& t, float speed, bool fromEnemy) : Sprite(scene, "bullet.png", t, Vector2(20, 20)), speed(speed), fromEnemy(fromEnemy)
{
    scene->AddActor(this);
    CircleCollider* col = new CircleCollider();
    col->radius = 10;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Bullet::Update(float dt)
{
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
        myScene->DestroyActor(this);
        return;
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
            myScene->DestroyActor(this);
        }
    }
    else
    {
        Enemy* e = dynamic_cast<Enemy*>(other);
        if (e)
        {
            e->TakeDamage(1);
            myScene->DestroyActor(this);
        }
    }
}
