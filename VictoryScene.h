#pragma once
#include "Scene.h"

/// <summary>
/// Escena mostrada al completar el juego
/// </summary>
class VictoryScene : public Scene
{
public:
    // Inicializa la escena con GI y fisicas
    VictoryScene(GraphicsInterface* GI, MyPhysics* mph);

    // Cuenta tiempo y vuelve al menu tras unos segundos o input
    void Update(float dt) override;
    // Dibuja los mensajes de victoria y puntuaciones
    void Render() override;

private:
    float timer = 0.0f; // Reloj interno
};