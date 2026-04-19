#include "VictoryScene.h"
#include "Game.h"
#include "Parameters.h"
#include "GraphicsInterface.h"
#include "MenuScene.h"

VictoryScene::VictoryScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
}

// Se encarga de volver al menu una vez pasan 7 segundos o al presionar la tecla espacio
void VictoryScene::Update(float dt)
{
    timer += dt;

    if (timer > 7.0f || Game::keyDown[SDLK_SPACE])
    {
        Game::newHS = false; // Resetamos el indicador de nuevo record
        Game::ChangeScene(new MenuScene(GI, mph));
    }
}

void VictoryScene::Render()
{
    // Fondo negro
    GI->ClearScreen(0, 0, 0);

    // Cargar el high score desde archivo
    int hs = Game::LoadHighScore();

    // Mensajes a mostrar
    std::string msg = "VICTORIA";
    std::string msg2 = "ENHORABUENA";
    std::string msg3 = ("SCORE " + std::to_string(Game::currentScore)).c_str();
    std::string msg4 = ("HIGH SCORE " + std::to_string(hs)).c_str();
    std::string msg5 = "HAS SUPERADO LA PUNTUACION MAXIMA";

    // Medir texto y centrarlo
    Vector2 size1 = GI->MeasureText(msg, EFONT_SIZE::BIG);
    Vector2 size2 = GI->MeasureText(msg2, EFONT_SIZE::BIG);
    Vector2 size3 = GI->MeasureText(msg3, EFONT_SIZE::MEDIUM);
    Vector2 size4 = GI->MeasureText(msg4, EFONT_SIZE::MEDIUM);
    Vector2 size5 = GI->MeasureText(msg5, EFONT_SIZE::MEDIUM);

    // Calcular posiciones centradas horizontalmente
    float x1 = (Parameters::screenWidth - size1.x) * 0.5f;
    float x2 = (Parameters::screenWidth - size2.x) * 0.5f;
    float x3 = (Parameters::screenWidth - size3.x) * 0.5f;
    float x4 = (Parameters::screenWidth - size4.x) * 0.5f;
    float x5 = (Parameters::screenWidth - size5.x) * 0.5f;

    // Posiciones verticales
    float y1 = Parameters::screenHeight * 0.35f;
    float y2 = Parameters::screenHeight * 0.45f;
    float y3 = Parameters::screenHeight * 0.55f;
    float y4 = Parameters::screenHeight * 0.65f;
    float y5 = Parameters::screenHeight * 0.75f;

    // Dibujar los textos
    GI->PrintText(msg, x1, y1, { 0,255,0 }, EFONT_SIZE::BIG);
    GI->PrintText(msg2, x2, y2, { 255,255,255 }, EFONT_SIZE::BIG);
    GI->PrintText(msg3, x3, y3, { 255,255,255 }, EFONT_SIZE::MEDIUM);
    GI->PrintText(msg4, x4, y4, { 255,255,255 }, EFONT_SIZE::MEDIUM);

    // Solo mostrar este mensaje si se ha superado el record
    if (Game::newHS)
        GI->PrintText(msg5, x5, y5, { 0,255,0 }, EFONT_SIZE::MEDIUM);

    // Renderizamos
    GI->DrawFrame();
}