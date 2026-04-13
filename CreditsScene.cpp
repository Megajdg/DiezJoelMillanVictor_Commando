#include "CreditsScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"

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

    GI->PrintText("CREDITOS", 50, 50, { 255,255,255 }, EFONT_SIZE::BIG);

    GI->PrintText("Desarrollado por:", 50, 150, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText("Joel Diez y Victor Millan", 50, 200, { 255,255,255 }, EFONT_SIZE::MEDIUM);

    GI->PrintText("Profesor: Francisco Arias", 50, 260, { 255,255,255 }, EFONT_SIZE::MEDIUM);

    GI->PrintText("Pulsa ESC para volver", 50, 400, { 255,0,0 }, EFONT_SIZE::MEDIUM);

    GI->DrawFrame();
}
