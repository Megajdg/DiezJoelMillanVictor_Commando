#include "AreaCompleteScene.h"
#include "Game.h"
#include "Parameters.h"
#include "GraphicsInterface.h"
#include "GameScene.h"

AreaCompleteScene::AreaCompleteScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber) : Scene(GI, mph), area(areaNumber)
{
}

void AreaCompleteScene::Update(float dt)
{
    timer += dt;

    // Espera 3 segundos o pulsa una tecla
    if (timer > 3.0f || Game::keyDown[SDLK_SPACE])
    {
        int nextArea = area + 1;
        if (nextArea > 3) nextArea = 1;

        // Cambiar a la nueva área
        Game::ChangeScene(new GameScene(GI, mph, nextArea));

        // Restaurar estado en la nueva escena
        GameScene* gs = (GameScene*)Game::instance->currentScene;
        Game::RestorePlayerState(gs);
    }
}

void AreaCompleteScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    std::string msg = "AREA " + std::to_string(area) + " COMPLETE";
    GI->PrintText(msg, Parameters::screenWidth * 0.3f, Parameters::screenHeight * 0.45f,
        { 255,255,255 }, EFONT_SIZE::BIG);

    GI->PrintText("GET READY...",
        Parameters::screenWidth * 0.35f,
        Parameters::screenHeight * 0.55f,
        { 255,255,255 }, EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}