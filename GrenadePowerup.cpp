#include "GrenadePowerup.h"
#include "Player.h"
#include "Scene.h"
#include "MyPhysics.h"
#include "AudioManager.h"
#include "AnimatedEntity.h"
#include "AnimationPowerUp.h"

GrenadePowerup::GrenadePowerup(Scene* scene, const Vector2& pos) : AnimatedEntity(scene, "powerUpGrenade.png", pos, Vector2(60, 60))
{
    // Registramos el actor en la escena con posicion
    scene->AddActor(this);

    // Cargamos las animaciones del spritesheet de proyectiles y activamos la animacion de bullet
    animationSet = LoadPowerUpAnimations();
    SetAnimation("grenadePU");

    // Añadimos un collider con trigger de radio 30
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
    // Solo reaccionar si el que entra es el jugador 
    Player* p = dynamic_cast<Player*>(other);
    if (!p)
        return;

    // Sonido de powerup
    AudioManager::instance().PlaySFX("powerup.wav");

    // Dar 3 granadas
    p->grenades += 3;

    // Destruir el pickup
    myScene->DestroyActor(this);
}