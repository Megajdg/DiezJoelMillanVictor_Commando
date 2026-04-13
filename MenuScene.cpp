#include "MenuScene.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"
#include "IntroScene.h"
#include "HelpScene.h"
#include "CreditsScene.h"
#include "GameScene.h"

MenuScene::MenuScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
}

void MenuScene::Update(float dt)
{
    static bool upPressedLastFrame = false;
    static bool downPressedLastFrame = false;
    static bool enterPressedLastFrame = false;
    static bool escPressedLastFrame = false;

    bool up = Game::keyDown[SDLK_UP];
    bool down = Game::keyDown[SDLK_DOWN];
    bool enter = Game::keyDown[SDLK_RETURN];
    bool esc = Game::keyDown[SDLK_ESCAPE];

    // --- UP ---
    if (up && !upPressedLastFrame)
        selected = (selected + 3) % 4;

    // --- DOWN ---
    if (down && !downPressedLastFrame)
        selected = (selected + 1) % 4;

    // --- ENTER ---
    if (enter && !enterPressedLastFrame)
    {
        switch (selected)
        {
        case 0: Game::ChangeScene(new GameScene(GI, mph)); break;
        case 1: Game::ChangeScene(new HelpScene(GI, mph)); break;
        case 2: Game::ChangeScene(new CreditsScene(GI, mph)); break;
        case 3: Game::ChangeScene(nullptr); break;
        }
    }

    if (esc && !escPressedLastFrame)
        Game::ChangeScene(nullptr); // Cerramos el juego

    // Guardar estado para el siguiente frame
    upPressedLastFrame = up;
    downPressedLastFrame = down;
    enterPressedLastFrame = enter;
    escPressedLastFrame = esc;
}

void MenuScene::Render()
{
    GI->ClearScreen(0, 0, 0);

    GI->PrintText("MENU PRINCIPAL", 100, 50, { 255,255,255 }, EFONT_SIZE::BIG);

    const char* options[4] = { "JUGAR", "AYUDA", "CREDITOS", "SALIR" };

    for (int i = 0; i < 4; i++)
    {
        bColor col = (i == selected) ? bColor{ 255,0,0 } : bColor{ 255,255,255 };
        GI->PrintText(options[i], 120, 150 + i * 60, col, EFONT_SIZE::MEDIUM);
    }

    GI->DrawFrame();
}
