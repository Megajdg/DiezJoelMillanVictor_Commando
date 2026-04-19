#pragma once
#include "Actor.h"
#include "Vector2.h"
#include <string>
#include "SDL3/SDL.h"

/// <summary>
/// Actor que representa una imagen en pantalla
/// </summary>
class Sprite : public Actor
{
protected:
    // Tamaño del sprite en pantalla
    Vector2 size;

public:
    // Nombre de la textura cargada en GraphicsInterface
    std::string image_name;

    // Crea un sprite con imagen y tamaño
    Sprite(Scene* myscene, std::string img_name, Vector2 size);

    // Constructor con transform inicial
    Sprite(Scene* myscene, std::string img_name, Transform transform, Vector2 size);

    virtual void Update(float deltaTime) override;
    virtual void Render() override;
};