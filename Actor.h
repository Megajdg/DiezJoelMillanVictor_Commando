#pragma once
#include "Transform.h"

/// <summary>
/// Clase base abstracta, todo lo que existe en escena deriva de aqui.
/// </summary>
class Actor
{
public:
	class Scene* myScene;	// Puntero a una escena
	bool isStatic = false;	// Determina si el actor se puede mover o no
	float mass = 1.f;		// Determina la masa del actor

	Actor(class Scene* myScene);					// Constructor
	Transform transform;							// Posicion, rotacion y escala del actor
	virtual void Update(float deltaTime);			// Logica del actor
	virtual void Render();							// Dibujo del actor
	virtual void OnTrigger(Actor* otherActor) {};	// Colision entre actores si es un Trigger
	virtual void OnHit(Actor* otherActor) {};		// Colision entre actores
};