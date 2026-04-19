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
    // Volver al menu pulsando ESC
    if (Game::keyDown[SDLK_ESCAPE])
        Game::ChangeScene(new MenuScene(GI, mph));
}

void CreditsScene::Render()
{
    // Pantalla en negro
    GI->ClearScreen(0, 0, 0);

    // Texto principal de la pantalla
    std::string title = "CREDITOS";

    // Calculamos el tamaño real del texto segun la fuente
    Vector2 titleSize = GI->MeasureText(title, EFONT_SIZE::BIG);

    // Centramos horizontalmente
    float titleX = (Parameters::screenWidth - titleSize.x) * 0.5f;

    // Posicionamos verticalmente
    float titleY = Parameters::screenHeight * 0.15f;

    // Dibujamos el texto
    GI->PrintText(title, titleX, titleY, { 255,255,255 }, EFONT_SIZE::BIG);

    // Texto principal de los creditos
    const char* lines[] = {
        "Desarrollado por:",
        "Joel Diez y Victor Millan",
        "Profesor: Francisco Arias"
    };

    // Calculamos el tamaño real de los textos, centramos, posicionamos y dibujamos
    for (int i = 0; i < 3; i++)
    {
        std::string txt = lines[i];
        Vector2 size = GI->MeasureText(txt, EFONT_SIZE::MEDIUM);

        float x = (Parameters::screenWidth - size.x) * 0.5f;
        float y = Parameters::screenHeight * 0.35f + i * 70;

        GI->PrintText(txt, x, y, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    }

    // Texto de volver de la pantalla
    std::string back = "Pulsa ESC para volver";

    // Calculamos el tamaño real del texto, centramos, posicionamos y dibujamos
    Vector2 backSize = GI->MeasureText(back, EFONT_SIZE::MEDIUM);

    float backX = (Parameters::screenWidth - backSize.x) * 0.5f;
    float backY = Parameters::screenHeight * 0.75f;

    GI->PrintText(back, backX, backY, { 255,0,0 }, EFONT_SIZE::MEDIUM);

    // Renderizamos
    GI->DrawFrame();
}