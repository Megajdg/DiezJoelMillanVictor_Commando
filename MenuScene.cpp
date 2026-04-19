#include "MenuScene.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "GraphicsInterface.h"
#include "IntroScene.h"
#include "HelpScene.h"
#include "CreditsScene.h"
#include "GameScene.h"
#include "AudioManager.h"
#include "Parameters.h"

MenuScene::MenuScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
    Game::nextExtraLifeScore = 10000;
    AudioManager::instance().playMusic("maintheme.mp3");

    GI->LoadImage("logo.png");
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
        case 0: Game::ChangeScene(new GameScene(GI, mph, 1)); break;
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

    // Tamaño real del logo
    Vector2 logoSize(600, 84);

    // Posición centrada
    float logoX = (Parameters::screenWidth - logoSize.x) * 0.5f;
    float logoY = Parameters::screenHeight * 0.15f;

    Transform t;
    t.position = Vector2(
        logoX + logoSize.x * 0.5f + Game::camera.position.x - Parameters::screenWidth * 0.5f,
        logoY + logoSize.y * 0.5f + Game::camera.position.y - Parameters::screenHeight * 0.5f
    );
    t.scale = Vector2(1, 1);
    t.rotation = 0;

    GI->DrawSprite("logo.png", t, logoSize);

    // --- OPCIONES ---
    const char* options[4] = { "JUGAR", "AYUDA", "CREDITOS", "SALIR" };

    for (int i = 0; i < 4; i++)
    {
        std::string opt = options[i];
        Vector2 optSize = GI->MeasureText(opt, EFONT_SIZE::MEDIUM);

        float x = (Parameters::screenWidth - optSize.x) * 0.5f;
        float y = Parameters::screenHeight * 0.35f + i * 80;

        bColor col = (i == selected) ? bColor{ 255,0,0 } : bColor{ 255,255,255 };

        GI->PrintText(opt, x, y, col, EFONT_SIZE::MEDIUM);
    }

    GI->DrawFrame();
}

