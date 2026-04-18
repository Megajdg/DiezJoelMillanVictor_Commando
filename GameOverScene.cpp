#include "GameOverScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "GraphicsInterface.h"
#include "Parameters.h"
#include "SDL3/SDL.h"

GameOverScene::GameOverScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
    backgroundColor = { 0, 0, 0 };
}

void GameOverScene::Update(float dt)
{
    timer += dt;

    if (timer > 5.0f || Game::keyDown[SDLK_SPACE])
    {
        Game::newHS = false;
        Game::ChangeScene(new MenuScene(GI, mph));
    }
}

void GameOverScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    int hs = Game::LoadHighScore();

    std::string msg = "FIN DE LA PARTIDA";
    std::string msg2 = ("SCORE " + std::to_string(Game::currentScore)).c_str();
    std::string msg3 = ("HIGH SCORE " + std::to_string(hs)).c_str();
    std::string msg4 = "HAS SUPERADO LA PUNTUACION MAXIMA";

    Vector2 size1 = GI->MeasureText(msg, EFONT_SIZE::BIG);
    Vector2 size2 = GI->MeasureText(msg2, EFONT_SIZE::MEDIUM);
    Vector2 size3 = GI->MeasureText(msg3, EFONT_SIZE::MEDIUM);
    Vector2 size4 = GI->MeasureText(msg4, EFONT_SIZE::MEDIUM);

    float x1 = (Parameters::screenWidth - size1.x) * 0.5f;
    float x2 = (Parameters::screenWidth - size2.x) * 0.5f;
    float x3 = (Parameters::screenWidth - size3.x) * 0.5f;
    float x4 = (Parameters::screenWidth - size4.x) * 0.5f;

    float y1 = Parameters::screenHeight * 0.35f;
    float y2 = Parameters::screenHeight * 0.45f;
    float y3 = Parameters::screenHeight * 0.55f;
    float y4 = Parameters::screenHeight * 0.65f;

    GI->PrintText(msg, x1, y1, { 255,0,0 }, EFONT_SIZE::BIG);
    GI->PrintText(msg2, x2, y2, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText(msg3, x3, y3, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    if (Game::newHS)
        GI->PrintText(msg4, x4, y4, { 0,255,0 }, EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}
