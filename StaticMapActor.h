#pragma once
#include "Actor.h"

/// <summary>
/// Representa el mapa estatico del nivel
/// </summary>
class StaticMapActor : public Actor
{
public:
    // Recibe la escena donde se registrara el mapa
    StaticMapActor(Scene* scene);
};