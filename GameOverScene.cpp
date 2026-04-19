#include "GameOverScene.h"
#include "Game.h"
#include "MenuScene.h"
#include "GraphicsInterface.h"
#include "Parameters.h"
#include "SDL3/SDL.h"

GameOverScene::GameOverScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
}

void GameOverScene::Update(float dt)
{
    // Hacemos correr el reloj interno
    timer += dt;

    // Volver al menu pulsando ESC o cuando acabe el temporizador
    if (timer > 5.0f || Game::keyDown[SDLK_SPACE])
    {
        // Marcamos tambien que NO hay un nuevo high score
        Game::newHS = false;
        Game::ChangeScene(new MenuScene(GI, mph));
    }
}

void GameOverScene::Render()
{
    // Pantalla en negro
    GI->ClearScreen(0, 0, 0);

    // Cargamos el high score
    int hs = Game::LoadHighScore();

    // Textos de la pantalla
    std::string msg = "FIN DE LA PARTIDA";
    std::string msg2 = ("SCORE " + std::to_string(Game::currentScore)).c_str();
    std::string msg3 = ("HIGH SCORE " + std::to_string(hs)).c_str();
    std::string msg4 = "HAS SUPERADO LA PUNTUACION MAXIMA";

    // Calculamos el tamaño real de los textos segun la fuente
    Vector2 size1 = GI->MeasureText(msg, EFONT_SIZE::BIG);
    Vector2 size2 = GI->MeasureText(msg2, EFONT_SIZE::MEDIUM);
    Vector2 size3 = GI->MeasureText(msg3, EFONT_SIZE::MEDIUM);
    Vector2 size4 = GI->MeasureText(msg4, EFONT_SIZE::MEDIUM);

    // Centramos horizontalmente
    float x1 = (Parameters::screenWidth - size1.x) * 0.5f;
    float x2 = (Parameters::screenWidth - size2.x) * 0.5f;
    float x3 = (Parameters::screenWidth - size3.x) * 0.5f;
    float x4 = (Parameters::screenWidth - size4.x) * 0.5f;

    // Posicionamos verticalmente
    float y1 = Parameters::screenHeight * 0.35f;
    float y2 = Parameters::screenHeight * 0.45f;
    float y3 = Parameters::screenHeight * 0.55f;
    float y4 = Parameters::screenHeight * 0.65f;

    // Dibujamos los textos menos el ultimo, salvo si has hecho high score, entonces los mostramos todos
    GI->PrintText(msg, x1, y1, { 255,0,0 }, EFONT_SIZE::BIG);
    GI->PrintText(msg2, x2, y2, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText(msg3, x3, y3, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    if (Game::newHS)
        GI->PrintText(msg4, x4, y4, { 0,255,0 }, EFONT_SIZE::MEDIUM);

    // Renderizamos
    GI->DrawFrame();
}
