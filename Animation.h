#pragma once
#include <vector>
#include <string>

/// <summary>
/// Define un frame con posicion (x, y) y tamaño (h, w)
/// </summary>
struct Frame
{
    float x, y, w, h;
};

/// <summary>
/// Gestiona como funcionan las animaciones
/// </summary>
class Animation
{
public:
    std::string name;                   // Nombre
    std::vector<Frame> frames;          // Lista de frames

    float frameTime = 0.1f;             // Tiempo que dura un frame
    float timer = 0.f;                  // Reloj interno
    int currentFrame = 0;               // Indice del frame actual

    bool loop = true;                   // Si la animacion hace loop
    bool finished = false;              // Si la animacion ha acabado

    void Reset();                       // Vuelve al primer frame
    void Update(float dt);              // Avanza la animacion segun el tiempo
    Frame GetCurrentFrame() const;      // Devuelve el frame actual
};
