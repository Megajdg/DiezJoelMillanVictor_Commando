#pragma once
#include "Collider.h"

/// <summary>
/// Define un collider circular
/// </summary>
class CircleCollider : public Collider
{
public:
	float radius{};		// Radio del collider
};