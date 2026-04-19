#pragma once
#include "Vector2.h"

/// <summary>
/// Transformacion de un objeto del mundo, posicion, rotacion y escala
/// </summary>
class Transform
{
public:
	// Constructor que inicializa solo la posicion, rotacion 0 y escala a 1
	Transform(const Vector2& pos) : position(pos), rotation(0), scale(1, 1) {}

	// Constructor por defecto posicion 0, rotacion 0 y escala 1
	Transform() : position(0, 0), rotation(0), scale(1, 1) {};

	// Posicion del objeto en el mundo
	Vector2 position{};

	// Rotacion en grados
	float rotation{};

	// Escala del objeto donde 1 es el tamaño original
	Vector2 scale = { 1.f };
};