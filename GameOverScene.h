#pragma once
#include "Scene.h"

/// <summary>
/// Escena de gameover
/// </summary>
class GameOverScene : public Scene
{
public:
    GameOverScene(GraphicsInterface* GI, MyPhysics* mph);   // Constructor

    void Update(float dt) override;                         // Logica de volver y del reloj interno
    void Render() override;                                 // Pantalla negra y texto

private:
    float timer = 0.0f;                                     // Reloj interno
};
