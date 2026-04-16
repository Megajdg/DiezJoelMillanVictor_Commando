#include "Grenade.h"
#include "Scene.h"
#include "Explosion.h"

Grenade::Grenade(Scene* scene, const Transform& t, Vector2 direction, Player* owner, bool fromEnemy) : Sprite(scene, "grenade.png", t, Vector2(20, 20)), fromEnemy(fromEnemy)
{
    this->owner = owner;
    this->fromEnemy = fromEnemy;
    scene->AddActor(this);

    direction = direction.normalize();

    float initialSpeed = 600.f;

    // Velocidad inicial en la dirección deseada
    velocity = direction * initialSpeed;

    // Impulso vertical extra para la parábola
    velocity.y -= 400.f;
}

void Grenade::Update(float dt)
{
    timer += dt;

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
