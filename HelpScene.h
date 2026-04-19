#pragma once
#include "Scene.h"

/// <summary>
/// Escena de ayuda
/// </summary>
class HelpScene : public Scene
{
public:
    HelpScene(GraphicsInterface* GI, MyPhysics* mph);   // Constructor

    void Update(float dt) override;                     // Logica de volver
    void Render() override;                             // Pantalla negra y texto
};
