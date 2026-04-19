#include "Explosion.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "Enemy.h"
#include "CircleCollider.h"
#include "AudioManager.h"
#include "AnimatedEntity.h"
#include "AnimationProjectile.h"

Explosion::Explosion(Scene* scene, const Transform& t, bool fromEnemy) : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100))
{
    // Registramos el actor en la escena con posicion, velocidad y si viene del enemigo o no
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);
    // Reproducimos el sonido de explosion
    AudioManager::instance().PlaySFX("explosion.wav");

    // Cargamos las animaciones del spritesheet de proyectiles y activamos la animacion de explosion
    animationSet = LoadProjectileAnimations();
    SetAnimation("explosion");

    // Añadimos un collider con trigger
    CircleCollider* col = new CircleCollider();
    col->radius = radius;
    col->isTrigger = true;
    scene->mph->AddCollider(col, this);
}

void Explosion::Update(float dt)
{
    AnimatedEntity::Update(dt);

    // Cuando termina la animacion, se destruye
    if (currentAnimation->finished)
        myScene->DestroyActor(this);
}

void Explosion::OnTrigger(Actor* other)
{
    // Si viene del enemigo, solo daña al jugador
    if (fromEnemy)
    {
        Player* p = dynamic_cast<Player*>(other);
        if (p)
            p->TakeDamage(1);
    }
    // Si viene del jugador, solo daña a enemigos
    else
    {
        Enemy* e = dynamic_cast<Enemy*>(other);
        if (e)
            e->TakeDamage(1);
    }
}