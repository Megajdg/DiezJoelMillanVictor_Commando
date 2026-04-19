#pragma once
#include "Collider.h"

/// <summary>
/// Collider rectangular
/// </summary>
class RectangleCollider : public Collider
{
public:
	Vector2 size{};	// Tamaño del rectangulo
};