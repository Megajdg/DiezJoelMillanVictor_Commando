#pragma once
#include "Scene.h"

/// <summary>
/// Escena de creditos
/// </summary>
class CreditsScene : public Scene
{
public:
    CreditsScene(GraphicsInterface* GI, MyPhysics* mph);    // Constructor

    void Update(float dt) override;                         // Logica de volver
    void Render() override;                                 // Pantalla negra y texto
};