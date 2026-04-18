#include "GrenadePowerup.h"
#include "Player.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "AudioManager.h"

GrenadePowerup::GrenadePowerup(Scene* scene, const Vector2& pos) : Sprite(scene, "grenade_powerup.png", pos, Vector2(60, 60))
{
    transform.position = pos;

    // Collider tipo trigger
    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    col->isTrigger = true;

    scene->mph->AddCollider(col, this);
    scene->AddActor(this);
}

void GrenadePowerup::OnTrigger(Actor* other)
{
    Player* p = dynamic_cast<Player*>(other);
    if (!p)
        return;

    AudioManager::instance().playSFX("powerup.wav");

    // Dar 3 granadas
    p->grenades += 3;

    // Destruir el pickup
    myScene->DestroyActor(this);
}
