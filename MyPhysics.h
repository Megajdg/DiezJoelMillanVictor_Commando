#pragma once

#include <vector>
#include <map>
#include "Vector2.h"

//#define DEBUG_COLLIDERS

typedef std::map<class Actor*, std::vector<class Collider*>*> ColliderMap;	// Mapa de vectores, lista de colliders

/// <summary>
/// Motor de fisicas, gestiona colisiones, triggers y separacion de actores
/// </summary>
class MyPhysics
{
	struct HIT_INFO
	{
		Vector2 col_p1, col_p2;					// Puntos de colision
		float minimum_allowed_distance = 0.f;	// 0 = a sin colision
		float current_distance;					// Distancia real
	};

	ColliderMap colliders_by_actor;				// Colliders registrados por actor

	MyPhysics();								// Constructor
	void Update();								// Gestiona colisiones

	// Separa actores segun masa y solapamiento
	void MoveActors(class Actor* a1, class Actor* a2, Vector2 current_a1_to_a2, float minimum_distance, float current_distance);

	// Decide tipo de colision
	HIT_INFO ManageCollision(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);

	// Colisiones especificas
	HIT_INFO IsCollidingCircleRectangle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);
	HIT_INFO IsCollidingCircleCircle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);
	HIT_INFO IsCollidingRectangleRectangle(class Collider* col1, class Actor* a1, class Collider* col2, class Actor* a2);

public:
	// Elimina todos los colliders de un actor
	void RemoveActor(class Actor*);

	// Añade un collider a un actor
	void AddCollider(class Collider* col, class Actor* act);

	// Elimina colliders del mapa estatico
	void RemoveMapColliders(Actor* mapActor);

	// Devuelve colliders para debug
	__forceinline ColliderMap GetCollidersByActor() const { return colliders_by_actor; }

	friend class Game; // Game controla el ciclo de fisicas
};