#pragma once
#include "Transform.h"
#include "Actor.h"

/// <summary>
/// Define la camara del juego
/// </summary>
class Camera
{
public:
	Camera(Vector2 initial_camera_position) : position(initial_camera_position) {};		// Inicializa la posicion de la camara

	void Update();																		// Logica de movimiento

	Vector2 position;																	// Posicion de la camara
	Actor *target = NULL;																// Actor que la camara sigue
};