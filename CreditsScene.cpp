#include "CreditsScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"
#include "Parameters.h"

CreditsScene::CreditsScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
}

void CreditsScene::Update(float dt)
{
    // Volver al menú con ESC
    if (Game::keyDown[SDLK_ESCAPE])
        Game::ChangeScene(new MenuScene(GI, mph));
}

void CreditsScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    // --- TÍTULO ---
    std::string title = "CREDITOS";
    Vector2 titleSize = GI->MeasureText(title, EFONT_SIZE::BIG);

    float titleX = (Parameters::screenWidth - titleSize.x) * 0.5f;
    float titleY = Parameters::screenHeight * 0.15f;

    GI->PrintText(title, titleX, titleY, { 255,255,255 }, EFONT_SIZE::BIG);

    // --- LÍNEAS DE CRÉDITOS ---
    const char* lines[] = {
        "Desarrollado por:",
        "Joel Diez y Victor Millan",
        "Profesor: Francisco Arias"
    };

    for (int i = 0; i < 3; i++)
    {
        std::string txt = lines[i];
        Vector2 size = GI->MeasureText(txt, EFONT_SIZE::MEDIUM);

        float x = (Parameters::screenWidth - size.x) * 0.5f;
        float y = Parameters::screenHeight * 0.35f + i * 70;

        GI->PrintText(txt, x, y, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    }

    // --- VOLVER ---
    std::string back = "Pulsa ESC para volver";
    Vector2 backSize = GI->MeasureText(back, EFONT_SIZE::MEDIUM);

    float backX = (Parameters::screenWidth - backSize.x) * 0.5f;
    float backY = Parameters::screenHeight * 0.75f;

    GI->PrintText(back, backX, backY, { 255,0,0 }, EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}