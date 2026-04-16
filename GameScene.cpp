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
#include "EnemySpawner.h"
#include "HUDWidget.h"

GameScene::GameScene(GraphicsInterface* GI, MyPhysics* mph) : Scene(GI, mph)
{
    // Fondo
    GI->LoadImage("area1_final.png");
    Sprite* bg = new Sprite(this, "area1_final.png", Vector2(1272, 6232));
    bg->transform.position = Vector2(0, -6232 * 0.5f);
    actors.insert(actors.begin(), bg);

    GenerateColliders();

    // Player
    GI->LoadImage("Joe.png");
    GI->LoadImage("player_spritesheet_final.png");
    Transform t;
    t.position = Vector2(0, -200);
    player = new Player(this, "player_spritesheet_final.png", t, 100);
    actors.push_back(player);

    Game::camera.position = player->transform.position;
    Game::camera.target = player;

    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    mph->AddCollider(col, player);

    new HUDWidget(this, player);

    new EnemySpawner(this);

    GI->LoadImage("bullet.png");

    GI->LoadImage("grenade.png");
    GI->LoadImage("explosion.png");
    GI->LoadImage("enemy.png");
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

void GameScene::AddScore(int amount)
{
    score += amount;
    Game::currentScore = score;
}

void GameScene::GenerateColliders()
{
    int w, h;
    auto mask = GI->LoadCollisionMask("area1_collidermap_final.png", w, h);
    if (mask.empty())
    {
        printf("ERROR: collision mask not loaded\n");
        return;
    }

    // Obtener info del mapa visual desde GraphicsInterface
    MapInfo info = GI->GetMapInfo("area1_final.png", 1272, 6232);

    float scaleX = info.scaleX;
    float scaleY = info.scaleY;

    int tileSize = 4;
    float radius = (tileSize * 0.5f) * scaleX;

    float mapOffsetX = -1272 * 0.5f;
    float mapOffsetY = -6232;

    StaticMapActor* mapActor = new StaticMapActor(this);

    for (int y = 0; y < h; y += tileSize)
    {
        for (int x = 0; x < w; x += tileSize)
        {
            bool hasCollision = false;

            for (int ty = 0; ty < tileSize && !hasCollision; ty++)
            {
                for (int tx = 0; tx < tileSize; tx++)
                {
                    int px = x + tx;
                    int py = y + ty;

                    if (px < w && py < h && mask[py * w + px] == 1)
                    {
                        hasCollision = true;
                        break;
                    }
                }
            }

            if (!hasCollision)
                continue;

            CircleCollider* col = new CircleCollider();
            col->radius = radius;

            col->relative_position = Vector2(
                (x + tileSize * 0.5f) * scaleX + mapOffsetX,
                (y + tileSize * 0.5f) * scaleY + mapOffsetY
            );

            mph->AddCollider(col, mapActor);
        }
    }
}