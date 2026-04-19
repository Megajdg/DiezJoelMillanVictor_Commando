#include "Explosion.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "Enemy.h"
#include "CircleCollider.h"
#include "AudioManager.h"

Explosion::Explosion(Scene* scene, const Transform& t, bool fromEnemy) : Sprite(scene, "explosion.png", t, Vector2(50, 50))
{
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);
    AudioManager::instance().playSFX("explosion.wav");

    CircleCollider* col = new CircleCollider();
    col->radius = radius;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Explosion::Update(float dt)
{
    timer += dt;
    if (!dying && timer >= duration) {
        dying = true;
        myScene->DestroyActor(this);
    }
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
