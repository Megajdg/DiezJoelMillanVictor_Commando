#include "GameScene.h"
#include "Player.h"
#include "Game.h"
#include "SDL3/SDL.h"
#include "Parameters.h"
#include "Sprite.h"
#include "MenuScene.h"
#include "GraphicsInterface.h"
#include "MyPhysics.h"
#include "CircleCollider.h"

GameScene::GameScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
    // Fondo
    GI->LoadImage("space.png");
    Sprite* bg = new Sprite(this, "space.png", 2000);
    bg->transform.position = Vector2(Parameters::width * 0.5f, Parameters::height * 0.5f);
    actors.push_back(bg);

    // Player
    GI->LoadImage("player_spritesheet.png");
    Transform t;
    t.position = Vector2(500, 350);
    player = new Player(this, "player_spritesheet.png", t, 100);
    actors.push_back(player);

    CircleCollider* col = new CircleCollider();
    col->radius = 50;
    mph->AddCollider(col, player);

    GI->LoadImage("bullet.png");

    // Cámara sigue al player
    Game::camera.target = player;
}

void GameScene::Update(float dt)
{
    // Volver al menú con ESC
    if (Game::keyDown[SDLK_ESCAPE])
    {
        Game::ChangeScene(new MenuScene(GI, mph));
        return;
    }

    Scene::Update(dt);
}

void GameScene::Render()
{
    GI->ClearScreen(0, 0, 20); // azul oscuro
    Scene::Render();
}
