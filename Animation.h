#pragma once

#include <vector>
#include <string>
#include "SDL3/SDL.h"

class Animation
{
public:
    Animation() = default;

    // Nombre opcional (por si quieres identificarla)
    std::string name;

    // Lista de frames (cada uno es un rectángulo dentro del spritesheet)
    std::vector<SDL_FRect> frames;

    // Tiempo que dura cada frame (en segundos)
    float frameTime = 0.1f;

    // ¿La animación se repite?
    bool loop = true;

    // Estado interno
    int currentFrame = 0;
    float timer = 0.0f;
    bool finished = false;

public:
    void Update(float deltaTime);
    const SDL_FRect& GetCurrentFrame() const;
    void Reset();
};
