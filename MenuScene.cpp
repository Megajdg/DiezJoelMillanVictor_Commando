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
    // Reiniciamos los puntos necesarios para obtener una vida
    Game::nextExtraLifeScore = 10000;

    // Hacemos sonar la musica del menu
    AudioManager::instance().PlayMusic("maintheme.mp3");

    // Cargamos el logo principal
    GI->LoadImage("logo.png");
}

void MenuScene::Update(float dt)
{
    // Variables para saber si se ha pulsado alguna tecla en el ultimo frame
    static bool upPressedLastFrame = false;
    static bool downPressedLastFrame = false;
    static bool enterPressedLastFrame = false;
    static bool escPressedLastFrame = false;

    // Variables para guardar si se esta pulsando una tecla
    bool up = Game::keyDown[SDLK_UP];
    bool down = Game::keyDown[SDLK_DOWN];
    bool enter = Game::keyDown[SDLK_RETURN];
    bool esc = Game::keyDown[SDLK_ESCAPE];

    // Si se esta pulsando hacia arriba y no se ha pulsado en el frame, subimos en la seleccion de opciones
    if (up && !upPressedLastFrame)
        selected = (selected + 3) % 4;

    // Si se esta pulsando hacia abajo y no se ha pulsado en el frame, bajamos en la seleccion de opciones
    if (down && !downPressedLastFrame)
        selected = (selected + 1) % 4;

    // Si se esta pulsando enter y no se ha pulsado en el frame, seleccionamos la opcion en la que nos encontramos
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

    // Salir del juego pulsando ESC
    if (esc && !escPressedLastFrame)
        Game::ChangeScene(nullptr);

    // Guardamos el estado para el siguiente frame
    upPressedLastFrame = up;
    downPressedLastFrame = down;
    enterPressedLastFrame = enter;
    escPressedLastFrame = esc;
}

void MenuScene::Render()
{
    // Pantalla en negro
    GI->ClearScreen(0, 0, 0);

    // Tamaño del logo
    Vector2 logoSize(600, 84);

    // Posición centrada
    float logoX = (Parameters::screenWidth - logoSize.x) * 0.5f;
    float logoY = Parameters::screenHeight * 0.15f;

    // Calculamos posicion donde ira el logo
    Transform t;
    t.position = Vector2(
        logoX + logoSize.x * 0.5f + Game::camera.position.x - Parameters::screenWidth * 0.5f,
        logoY + logoSize.y * 0.5f + Game::camera.position.y - Parameters::screenHeight * 0.5f
    );
    t.scale = Vector2(1, 1);
    t.rotation = 0;

    // Lo pintamos en la posicion deseada
    GI->DrawSprite("logo.png", t, logoSize);

    // Texto principal de opciones de menu
    const char* options[4] = { "JUGAR", "AYUDA", "CREDITOS", "SALIR" };

    // Calculamos el tamaño real de los textos, centramos, posicionamos y dibujamos
    for (int i = 0; i < 4; i++)
    {
        std::string opt = options[i];
        Vector2 optSize = GI->MeasureText(opt, EFONT_SIZE::MEDIUM);

        float x = (Parameters::screenWidth - optSize.x) * 0.5f;
        float y = Parameters::screenHeight * 0.35f + i * 80;

        bColor col = (i == selected) ? bColor{ 255,0,0 } : bColor{ 255,255,255 };

        GI->PrintText(opt, x, y, col, EFONT_SIZE::MEDIUM);
    }

    // Renderizamos
    GI->DrawFrame();
}