#include "Grenade.h"
#include "Scene.h"
#include "Explosion.h"
#include "AnimationProjectile.h"
#include "AnimatedEntity.h"

Grenade::Grenade(Scene* scene, const Transform& t, Vector2 direction, Player* owner, bool fromEnemy) : AnimatedEntity(scene, "projectiles.png", t, Vector2(100, 100)), fromEnemy(fromEnemy)
{
    // Registramos el actor en la escena con posicion, velocidad y si viene del enemigo o no
    this->owner = owner;
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);

    // Cargamos las animaciones del spritesheet de proyectiles y activamos la animacion de grenade
    animationSet = LoadProjectileAnimations();
    SetAnimation("grenade");

    // Normalizamos direccion
    direction = direction.normalize();
    float initialSpeed = 300.f;

    // Velocidad inicial en la direccion deseada
    velocity = direction * initialSpeed;

    // Impulso vertical extra para la parabola
    velocity.y -= 400.f;
}

void Grenade::Update(float dt)
{
    // Hacemos correr el reloj interno
    timer += dt;

    AnimatedEntity::Update(dt);

    // Fisica simple parabolica
    velocity.y += gravity * dt;
    transform.position += velocity * dt;

    // Explota al terminar el tiempo
    if (!exploded && timer >= fuseTime)
        Explode();
}

void Grenade::Explode()
{
    // Si ya ha explotado, abortamos
    if (exploded) return;
    exploded = true;

    // Crear explosion, con una posicion designada
    Transform t;
    t.position = transform.position;
    t.rotation = 0;
    new Explosion(myScene, t, fromEnemy);

    // Eliminar granada
    myScene->DestroyActor(this);

    // Liberar flag en el jugador si la lanzo él
    if (owner)
        owner->grenadeActive = false;
}