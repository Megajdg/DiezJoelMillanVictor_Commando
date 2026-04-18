#include "AreaCompleteScene.h"
#include "Game.h"
#include "Parameters.h"
#include "GraphicsInterface.h"
#include "GameScene.h"
#include "VictoryScene.h"
#include "AudioManager.h"

AreaCompleteScene::AreaCompleteScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber) : Scene(GI, mph), area(areaNumber)
{
    AudioManager::instance().playMusic("stageclear.mp3");
}

void AreaCompleteScene::Update(float dt)
{
    timer += dt;

    // Espera 8 segundos o pulsa una tecla
    if (timer > 8.0f || Game::keyDown[SDLK_SPACE])
    {
        if (area == 3)
        {
            if (Game::currentScore > Game::LoadHighScore()) {
                Game::newHS = true;
                Game::SaveHighScore(Game::currentScore);
            }
            Game::ChangeScene(new VictoryScene(GI, mph));
            return;
        }

        int nextArea = area + 1;
        Game::ChangeScene(new GameScene(GI, mph, nextArea));

        // Restaurar estado en la nueva escena
        GameScene* gs = (GameScene*)Game::instance->currentScene;
        Game::RestorePlayerState(gs);
    }
}

void AreaCompleteScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    std::string msg = "AREA " + std::to_string(area) + " COMPLETADA";
    std::string msg2 = "PREPARATE...";

    // Medir textos
    Vector2 size1 = GI->MeasureText(msg, EFONT_SIZE::BIG);
    Vector2 size2 = GI->MeasureText(msg2, EFONT_SIZE::MEDIUM);

    // Calcular posiciones centradas
    float x1 = (Parameters::screenWidth - size1.x) * 0.5f;
    float x2 = (Parameters::screenWidth - size2.x) * 0.5f;

    float y1 = Parameters::screenHeight * 0.45f;
    float y2 = Parameters::screenHeight * 0.55f;

    // Dibujar textos centrados
    GI->PrintText(msg, x1, y1, { 255,255,255 }, EFONT_SIZE::BIG);
    if (Game::newHS) {
        GI->PrintText(msg2, x2, y2, { 255,255,255 }, EFONT_SIZE::MEDIUM);
        Game::newHS = false;
    }

    GI->DrawFrame();
}