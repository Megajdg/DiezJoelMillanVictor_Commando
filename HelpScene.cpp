#include "HelpScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"
#include "Parameters.h"

HelpScene::HelpScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
}

void HelpScene::Update(float dt)
{
    // Volver al menú con ESC
    if (Game::keyDown[SDLK_ESCAPE])
        Game::ChangeScene(new MenuScene(GI, mph));
}

void HelpScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    // --- TÍTULO ---
    std::string title = "AYUDA";
    Vector2 titleSize = GI->MeasureText(title, EFONT_SIZE::BIG);

    float titleX = (Parameters::screenWidth - titleSize.x) * 0.5f;
    float titleY = Parameters::screenHeight * 0.15f;

    GI->PrintText(title, titleX, titleY, { 255,255,255 }, EFONT_SIZE::BIG);

    // --- LÍNEAS DE AYUDA ---
    const char* lines[] = {
        "W/S: Avanzar / Retroceder",
        "A/D: Girar nave",
        "ESPACIO: Disparar",
        "G: Lanzar granada"
    };

    for (int i = 0; i < 4; i++)
    {
        std::string txt = lines[i];
        Vector2 size = GI->MeasureText(txt, EFONT_SIZE::MEDIUM);

        float x = (Parameters::screenWidth - size.x) * 0.5f;
        float y = Parameters::screenHeight * 0.30f + i * 70;

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