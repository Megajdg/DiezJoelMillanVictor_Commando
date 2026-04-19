#include "AreaCompleteScene.h"
#include "Game.h"
#include "Parameters.h"
#include "GraphicsInterface.h"
#include "GameScene.h"
#include "VictoryScene.h"
#include "AudioManager.h"

AreaCompleteScene::AreaCompleteScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber) : Scene(GI, mph), area(areaNumber)
{
    // Reproduce la cancion de area completada
    AudioManager::instance().PlayMusic("stageclear.mp3");
}

void AreaCompleteScene::Update(float dt)
{
    // Hacemos correr el reloj interno
    timer += dt;

    // Espera 8 segundos espacio para salir de la escena
    if (timer > 8.0f || Game::keyDown[SDLK_SPACE])
    {
        // Si el area que has terminado es la 3
        if (area == 3)
        {
            // Comprobamos si hay un nuevo highscore, si lo hay lo marcamos y lo guardamos
            if (Game::currentScore > Game::LoadHighScore()) {
                Game::newHS = true;
                Game::SaveHighScore(Game::currentScore);
            }

            // Activamos la escena de victoria y abortamos
            Game::ChangeScene(new VictoryScene(GI, mph));
            return;
        }

        // Cargamos la escena de juego con la siguiente area
        int nextArea = area + 1;
        Game::ChangeScene(new GameScene(GI, mph, nextArea));

        // Obtenemos la nueva escena recien creada y restauramos los datos del jugador (vidas, granadas y puntuacion)
        GameScene* gs = (GameScene*)Game::instance->currentScene;
        Game::RestorePlayerState(gs);
    }
}

void AreaCompleteScene::Render()
{
    // Pantalla en negro
    GI->ClearScreen(0, 0, 0);

    // Texto principal y secundario de la pantalla
    std::string msg = "AREA " + std::to_string(area) + " COMPLETADA";
    std::string msg2 = "PREPARATE...";

    // Calculamos el tamaño real del texto segun la fuente
    Vector2 size1 = GI->MeasureText(msg, EFONT_SIZE::BIG);
    Vector2 size2 = GI->MeasureText(msg2, EFONT_SIZE::MEDIUM);

    // Centramos horizontalmente
    float x1 = (Parameters::screenWidth - size1.x) * 0.5f;
    float x2 = (Parameters::screenWidth - size2.x) * 0.5f;

    // Posicionamos verticalmente
    float y1 = Parameters::screenHeight * 0.45f;
    float y2 = Parameters::screenHeight * 0.55f;

    // Dibujamos los textos salvo si vienes de area 3, que solo mostramos uno
    GI->PrintText(msg, x1, y1, { 255,255,255 }, EFONT_SIZE::BIG);
    if (area != 3)
        GI->PrintText(msg2, x2, y2, { 255,255,255 }, EFONT_SIZE::MEDIUM);

    // Renderizamos
    GI->DrawFrame();
}