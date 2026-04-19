#include "EnemySpawner.h"
#include "Enemy.h"
#include "Scene.h"
#include "Game.h"
#include "Parameters.h"
#include "GameScene.h"
#include "GrenadierEnemy.h"
#include "ShooterEnemy.h"
#include "HybridEnemy.h"

EnemySpawner::EnemySpawner(Scene* scene) : Actor(scene)
{
    scene->AddActor(this);
}

void EnemySpawner::Update(float dt)
{
    timer += dt;

    if (timer >= spawnRate)
    {
        timer = 0.f;

        GameScene* gs = (GameScene*)myScene;

        float camX = Game::camera.position.x;
        float camY = Game::camera.position.y;

        // BORDES REALES DE PANTALLA EN COORDENADAS DEL MUNDO
        float worldLeft = camX - Parameters::screenWidth * 0.5f;
        float worldRight = camX + Parameters::screenWidth * 0.5f;
        float worldTop = camY - Parameters::screenHeight * 0.75f;
        float worldBottom = camY + Parameters::screenHeight * 0.25f;

        // ZONAS DE SPAWN FUERA DE PANTALLA
        Vector2 spawnTop(
            camX + (rand() % Parameters::screenWidth) - Parameters::screenWidth * 0.5f,
            worldTop + 100
        );

        Vector2 spawnLeft(
            worldLeft,
            camY - (rand() % 400)
        );

        Vector2 spawnRight(
            worldRight,
            camY - (rand() % 400)
        );

        // Elegir zona
        int side = rand() % 3;
        Vector2 spawnPos =
            (side == 0) ? spawnTop :
            (side == 1) ? spawnLeft :
            spawnRight;

        float probShooter = 0.5f;   // 50%
        float probGrenadier = 0.3f; // 30%
        float probHybrid = 0.2f;    // 20%

        int r = rand() % 100;

        if (r < probShooter * 100)
        {
            new ShooterEnemy(myScene, Transform(spawnPos), gs->GetPlayer());
        }
        else if (r < (probShooter + probGrenadier) * 100)
        {
            new GrenadierEnemy(myScene, Transform(spawnPos), gs->GetPlayer());
        }
        else
        {
            new HybridEnemy(myScene, Transform(spawnPos), gs->GetPlayer());
        }

    }
}
