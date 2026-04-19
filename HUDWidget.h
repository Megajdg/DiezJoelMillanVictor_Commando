#pragma once
#include "Widget.h"
#include "Player.h"

/// <summary>
/// Widget del HUD, muestra score, vidas y high score
/// </summary>
class HUDWidget : public Widget
{
public:
    Player* player = nullptr;                   // Referencia al jugador
    class GameScene* gameScene = nullptr;       // Para acceder al score

    HUDWidget(Scene* scene, Player* player);    // Registrar HUD en la UI
    void Render() override;                     // Dibujo del HUD
};