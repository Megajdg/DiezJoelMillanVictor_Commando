#pragma once
#include "Vector2.h"

/// <summary>
/// Define un collider
/// </summary>
class Collider
{
public:
	bool isTrigger{};				// Para saber si es trigger o no
	Vector2 relative_position{};	// Offset del collider respecto al centro del actor

	virtual ~Collider() {};			// Destructor
};