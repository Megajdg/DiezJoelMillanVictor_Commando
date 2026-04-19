#pragma once
#include "Scene.h"

/// <summary>
/// Escena de area completada
/// </summary>
class AreaCompleteScene : public Scene
{
public:
    AreaCompleteScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber);   // Constructor

    void Update(float dt) override;                                             // Logica de transicion
    void Render() override;                                                     // Pantalla negra y texto

private:
    float timer = 0.0f;                                                         // Reloj interno
    int area = 1;                                                               // Numero de area completada
};