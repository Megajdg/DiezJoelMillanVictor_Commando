#include "GrenadePowerup.h"
#include "Player.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "AudioManager.h"
#include "AnimatedEntity.h"
#include "AnimationPowerUp.h"

GrenadePowerup::GrenadePowerup(Scene* scene, const Vector2& pos) : AnimatedEntity(scene, "powerUpGrenade.png", pos, Vector2(60, 60))
{

    animationSet = LoadPowerUpAnimations();
    SetAnimation("grenadePU");

    scene->AddActor(this);

    // Collider tipo trigger
    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    col->isTrigger = true;

    scene->mph->AddCollider(col, this);
}

void GrenadePowerup::Update(float dt)
{
    AnimatedEntity::Update(dt);
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
