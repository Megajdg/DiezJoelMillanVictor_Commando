#include <sstream>
#include "HUDWidget.h"
#include "GameScene.h"
#include "Game.h"
#include "GraphicsInterface.h"
#include "Parameters.h"

template<typename T>
std::string ToString(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

std::string Pad(int value, int digits)
{
    std::string s = std::to_string(value);
    while (s.length() < digits) s = "0" + s;
    return s;
}

HUDWidget::HUDWidget(Scene* scene, Player* player) : Widget(scene)
{
    this->player = player;
    this->gameScene = (GameScene*)scene;

    scene->AddUI(this);
}

void HUDWidget::Render()
{
    auto GI = myScene->GI;

    int hudWidth = 1270;
    int x = (Parameters::screenWidth - hudWidth) /2;
    int y = Parameters::screenHeight * 0.66f;
    float hudHeight = 1000;
    GI->DrawRectScreen(
        0,
        y - 20,
        Parameters::screenWidth,
        hudHeight,
        0, 0, 0
    );


    // SCORE
    std::string scoreStr = "SCORE " + Pad(gameScene->score, 6);

    // GRANADAS
    std::string grenStr = " = " + Pad(player->grenades, 2);

    // VIDAS
    std::string menStr = "MEN " + Pad(player->lives, 2);

    // HIGH SCORE
    std::string hiStr = "HI " + Pad(Game::LoadHighScore(), 6);

    // Dibujo estilo Commodore: todo en una línea
    GI->PrintText(scoreStr, x, y, { 255,255,255 }, EFONT_SIZE::BIG);
    GI->PrintText("Q", x + 450, y, { 255,255,255 }, EFONT_SIZE::BIG, true);
    GI->PrintText(grenStr, x + 450, y, { 255,255,255 }, EFONT_SIZE::BIG);
    GI->PrintText(menStr, x + 690, y, { 255,255,255 }, EFONT_SIZE::BIG);
    GI->PrintText(hiStr, x + 980, y, { 255,255,255 }, EFONT_SIZE::BIG);
}
