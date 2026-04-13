#include "HelpScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"

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

    GI->PrintText("AYUDA", 50, 50, { 255,255,255 }, EFONT_SIZE::BIG);

    GI->PrintText("W/S: Avanzar / Retroceder", 50, 150, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText("A/D: Girar nave", 50, 200, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText("ESPACIO: Disparar", 50, 250, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText("G: Lanzar granada", 50, 300, { 255,255,255 }, EFONT_SIZE::MEDIUM);

    GI->PrintText("Pulsa ESC para volver", 50, 400, { 255,0,0 }, EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}
