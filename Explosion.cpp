#include "Explosion.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "Enemy.h"
#include "CircleCollider.h"
#include "AnimatedEntity.h"
#include "AnimationProjectile.h"


Explosion::Explosion(Scene* scene, const Transform& t, bool fromEnemy) 
    : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100))
{
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);

    animationSet = LoadProjectileAnimations();
    SetAnimation("explosion");

    CircleCollider* col = new CircleCollider();
    col->radius = radius;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Explosion::Update(float dt)
{
    AnimatedEntity::Update(dt);

    //cuando termina la animacion se destruye
    if (currentAnimation->finished)
        myScene->DestroyActor(this);
}

void Explosion::OnTrigger(Actor* other)
{
    if (fromEnemy)
    {
        // Solo daña al jugador
        Player* p = dynamic_cast<Player*>(other);
        if (p)
            p->TakeDamage(1);
    }
    else
    {
        // Solo daña enemigos
        Enemy* e = dynamic_cast<Enemy*>(other);
        if (e)
            e->TakeDamage(1);
    }
}
