#pragma once

#include "Vector2.h"

class Transform
{
public:
	Transform() : position(0, 0), rotation(0), scale(1, 1) {};
	Vector2 position{};
	float rotation{};
	Vector2 scale = {1.f};
};

