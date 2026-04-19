#include "Grenade.h"
#include "Scene.h"
#include "Explosion.h"
#include "AnimationProjectile.h"
#include "AnimatedEntity.h"

Grenade::Grenade(Scene* scene, const Transform& t, Vector2 direction, Player* owner, bool fromEnemy)
    : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100)),fromEnemy(fromEnemy)
{
    this->owner = owner;
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);

    animationSet = LoadProjectileAnimations();
    SetAnimation("grenade");

    direction = direction.normalize();

    float initialSpeed = 300.f;

    // Velocidad inicial en la dirección deseada
    velocity = direction * initialSpeed;

    // Impulso vertical extra para la parábola
    velocity.y -= 400.f;
}

void Grenade::Update(float dt)
{
    timer += dt;
    AnimatedEntity::Update(dt);

    // Física simple parabólica
    velocity.y += gravity * dt;
    transform.position += velocity * dt;

    // Explota al terminar el tiempo
    if (!exploded && timer >= fuseTime)
        Explode();
}

void Grenade::Explode()
{
    if (exploded) return;
    exploded = true;

    // Crear explosión
    Transform t;
    t.position = transform.position;
    t.rotation = 0;
    new Explosion(myScene, t, fromEnemy);

    myScene->DestroyActor(this);
    if (owner)
        owner->grenadeActive = false;
}
