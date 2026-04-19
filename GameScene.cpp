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
#include "GrenadePowerup.h"
#include "AreaCompleteScene.h"
#include "AudioManager.h"

GameScene::GameScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber) : Scene(GI, mph), area(areaNumber)
{
    // Reproducimos la musica del juego
    AudioManager::instance().PlayMusic("maintheme.mp3");

    // Cargamos todas las imagenes del juego
    GI->LoadImage("Shooter.png");
    GI->LoadImage("Grenadier.png");
    GI->LoadImage("Hybrid.png");
    GI->LoadImage("player_spritesheet_final.png");
    GI->LoadImage("projectiles.png");
    GI->LoadImage("powerUpGrenade.png");
    GI->LoadImage("grenade_powerup.png");
    GI->LoadImage("bullet.png");
    GI->LoadImage("grenade.png");
    GI->LoadImage("explosion.png");

    // Spawneamos al player en la posicion indicada
    Transform t;
    t.position = Vector2(0, -200);
    player = new Player(this, "player_spritesheet_final.png", t, 100);
    actors.push_back(player);

    // Anclamos la camara al jugador
    Game::camera.position = player->transform.position;
    Game::camera.target = player;

    // Le añadimos un collider al jugador
    CircleCollider* col = new CircleCollider();
    col->radius = 30;
    mph->AddCollider(col, player);

    // Instanciamos el HUD y el spawner de enemigos
    new HUDWidget(this, player);
    new EnemySpawner(this);

    // Strings para la carga de mapas
    std::string mapImage;
    std::string collisionMask;

    // Guardamos el mapa correspondiente segun el area, y tambien su mapa de colisiones
    switch (area)
    {
    case 1:
        mapImage = "area1_final.png";
        collisionMask = "area1_collidermap_final.png";
        break;

    case 2:
        mapImage = "area2_final.png";
        collisionMask = "area2_collidermap_final.png";
        break;

    case 3:
        mapImage = "area3_final.png";
        collisionMask = "area3_collidermap_final.png";
        break;
    }

    // Instanciamos el mapa seleccionado en la posicion deseada
    GI->LoadImage(mapImage);
    Sprite* bg = new Sprite(this, mapImage, Vector2(1272, 6232));
    bg->transform.position = Vector2(0, -6232 * 0.5f);
    actors.insert(actors.begin(), bg);

    // Creamos un actor para el mapa y generamos sus colliders
    mapActor = new StaticMapActor(this);
    GenerateColliders(mapImage, collisionMask);

    // Indicamos donde deben spawnear los powerups segun el mapa que toque
    switch (area)
    {
    case 1:
        grenadePickupPositions = {
            {  300,  -1700 },
            {  400,  -3000 },
            { -500,  -4600 },
            { -550,  -5050 },
            { 400,  -5700 }
        };
        break;

    case 2:
        grenadePickupPositions = {
            {  -500, -1000 },
            { 500, -2000 },
            {  250, -2500 }
        };
        break;

    case 3:
        grenadePickupPositions = {
            { -400, -1800 },
            {  400, -3700 },
            {  400, -5400 }
        };
        break;
    }
    
    // Instanciamos cada powerup en su posicion deseada
    for (auto& pos : grenadePickupPositions)
        new GrenadePowerup(this, pos);
}

void GameScene::Update(float dt)
{
    // Volver al menu con ESC
    if (Game::keyDown[SDLK_ESCAPE])
    {
        Game::ChangeScene(new MenuScene(GI, mph));
        return;
    }

    Scene::Update(dt);

    // Si el jugador llega al final del mapa, cargamos la escena de final de area
    if (player->transform.position.y < -6100)
    {
        Game::SavePlayerState(player);
        Game::ChangeScene(new AreaCompleteScene(GI, mph, area));
        return;
    }

    // Si conseguimos los puntos necesarios, obtenemos una vida extra
    if (score >= Game::nextExtraLifeScore)
    {
        player->lives++;
        // Sonido de powerup
        AudioManager::instance().PlaySFX("powerup.wav");

        // Preparar la siguiente puntuacion a alcanzar para conseguir una vida
        Game::nextExtraLifeScore += 10000;
    }
}

void GameScene::Render()
{
    // Pantalla en negro
    GI->ClearScreen(0, 0, 0);

    Scene::Render();
}

void GameScene::AddScore(int amount)
{
    // Actualizamos score
    score += amount;
    Game::currentScore = score;
}

void GameScene::GenerateColliders(std::string mapImage, std::string collisionMask)
{
    // Cargamos un array lineal de w * h, donde 1 es colision y 0 es libre
    int w, h;
    auto mask = GI->LoadCollisionMask(collisionMask, w, h);
    if (mask.empty())
    {
        printf("ERROR: collision mask not loaded\n");
        return;
    }

    // Obtener info del mapa visual desde GraphicsInterface
    MapInfo info = GI->GetMapInfo(mapImage, 1272, 6232);

    // Aplicamos la escala que tenga el mapa en el juego
    float scaleX = info.scaleX;
    float scaleY = info.scaleY;

    // Creamos un collider por cada 16 pixeles (tilemap de 4x4)
    int tileSize = 4;
    float radius = (tileSize * 0.5f) * scaleX;

    // Alineamos la mascara con el mapa
    float mapOffsetX = -1272 * 0.5f;
    float mapOffsetY = -6232;

    // Recorremos todo el tilemap, generando un collider si en el tile hay un solo pixel de colision
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

            // Creamos el collider circular con el radio de las tiles
            CircleCollider* col = new CircleCollider();
            col->radius = radius;

            // Lo colocamos en el centro del tile
            col->relative_position = Vector2(
                (x + tileSize * 0.5f) * scaleX + mapOffsetX,
                (y + tileSize * 0.5f) * scaleY + mapOffsetY
            );

            // Y lo registramos en el motor de fisicas
            mph->AddCollider(col, mapActor);
        }
    }
}