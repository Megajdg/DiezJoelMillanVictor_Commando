#pragma once
#include "AnimatedEntity.h"
#include "Weapon.h"

// Clase principal del jugador
// Controla movimeinto, animaciones, disparo, graanadas y vida

/// <summary>
/// Clase principal del jugador, controla movimiento, animaciones, disparo, graanadas y vida
/// </summary>
class Player : public AnimatedEntity
{
public:
	Player(Scene* myscene, std::string img_name, float side_size);						// Constructor
	Player(Scene* myscene, std::string img_name, Transform transform, float side_size);	// Constructor con transform

	int lives = 5;																		// Vidas jugador
	int grenades = 5;																	// Granadas jugador

	virtual void Update(float deltaTime);	
	bool grenadeActive = false;				
	void TakeDamage(int dmg);				

private:
	void UpdateMovement(float deltaTime);	
	std::string lastMoveAnim = "run_front";	
	float linear_speed = 300.f;				
	float ang_speed = 100.f;				
	Weapon* weapon = nullptr;				
	bool isThrowing = false;				
	bool isDead = false;					
	bool dying = false;						
	float deathTimer = 0.f;					
};