#include "GameOverScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "GraphicsInterface.h"
#include "Parameters.h"
#include "SDL3/SDL.h"

GameOverScene::GameOverScene(GraphicsInterface* GI, MyPhysics* mph)
    : Scene(GI, mph)
{
    backgroundColor = { 0, 0, 0 };
}

void GameOverScene::Update(float dt)
{
    Scene::Update(dt);

    if (Game::keyDown[SDLK_RETURN])
    {
        Game::keyDown[SDLK_RETURN] = false;
        Game::ChangeScene(new MenuScene(GI, mph));
    }
}

void GameOverScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    int hs = Game::LoadHighScore();

    // Título grande estilo menú
    GI->PrintText("GAME OVER",
        100, 80,
        { 255, 255, 255 },
        EFONT_SIZE::BIG);

    // SCORE
    GI->PrintText(
        ("SCORE " + std::to_string(Game::currentScore)).c_str(),
        120, 200,
        { 255, 255, 255 },
        EFONT_SIZE::MEDIUM);

    // HIGH SCORE
    GI->PrintText(
        ("HI " + std::to_string(hs)).c_str(),
        120, 260,
        { 255, 255, 255 },
        EFONT_SIZE::MEDIUM);

    // Instrucción
    GI->PrintText(
        "PULSA ENTER PARA VOLVER AL MENU",
        120, 350,
        { 255, 0, 0 },
        EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}
