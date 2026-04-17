#pragma once
#include "Sprite.h"
#include "Player.h"

class Player;

enum class EnemyState
{
    MOVING,
    SHOOTING
};

class Enemy : public AnimatedEntity
{
public:
    Enemy(Scene* scene, const Transform& t, Player* target, const std::string& spritesheet);
    Player* target = nullptr;

    float speed = 200.f;
    int health = 1;

    float shootCooldown = 1.5f;
    float shootTimer = 0.f;

    bool hasShot = false;

    EnemyState state = EnemyState::MOVING;
    float stateTimer = 0.f;
    Vector2 moveDir = { 0,0 };

    virtual void ShootAtPlayer() {}

    void Update(float dt) override;
    void UpdateMoving(float dt);
    void UpdateShooting(float dt);
    void ChooseNewDirection();
    void TakeDamage(int dmg);
};
