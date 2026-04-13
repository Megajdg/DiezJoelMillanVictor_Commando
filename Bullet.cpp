#include "Bullet.h"
#include "Scene.h"
#include "Game.h"
#include "CircleCollider.h"
#include "MyPhysics.h"

Bullet::Bullet(Scene* scene, const Transform& t, float speed) : Sprite(scene, "bullet.png", t, Vector2(20, 20)), speed(speed)
{
    scene->AddActor(this);
    CircleCollider* col = new CircleCollider();
    col->radius = 10;
    scene->mph->AddCollider(col, this);
}

void Bullet::Update(float dt)
{
    // Avanza en la dirección del ángulo del transform
    float rad = transform.rotation * 3.14159f / 180.f;
    transform.position.x += cos(rad) * speed * dt;
    transform.position.y += sin(rad) * speed * dt;

    // Si sale de la pantalla destruir
    if (transform.position.x < -100 || transform.position.x > 2000 ||
        transform.position.y < -100 || transform.position.y > 2000)
    {
        myScene->DestroyActor(this);
    }
}
