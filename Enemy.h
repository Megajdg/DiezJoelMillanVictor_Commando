#pragma once
#include "Sprite.h"
#include "Player.h"

// Forward declaration al Player
class Player;

/// <summary>
/// Define posible estados del enemigo
/// </summary>
enum class EnemyState
{
    MOVING,
    SHOOTING
};

/// <summary>
/// Clase base para los enemigos, controla movimiento, IA, animaciones, disparo y muerte
/// </summary>
class Enemy : public AnimatedEntity
{
public:
    Enemy(Scene* scene, const Transform& t, Player* target, const std::string& spritesheet);
    Player* target = nullptr;

    float speed = 200.f;    // Velocidad enemigo
    int health = 1;         // Vida enemigo

    float shootCooldown = 1.5f;     // Tiempo entre disparos
    float shootTimer = 0.f;         // Temporizador interno

    // Evita disparar mas de una vez por ciclo
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

    bool isDead = false;
    bool dying = false;
    float deathTimer = 0.f;
};