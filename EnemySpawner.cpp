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
    // Añadimos el actor a la escena
    scene->AddActor(this);
}

void EnemySpawner::Update(float dt)
{
    // Hacemos correr el reloj
    timer += dt;

    // Cada vez que pasa el tiempo de aparicion
    if (timer >= spawnRate)
    {
        // Reiniciamos temporizador
        timer = 0.f;

        // Obtenemos la escena de juego y la camara
        GameScene* gs = (GameScene*)myScene;

        float camX = Game::camera.position.x;
        float camY = Game::camera.position.y;

        // Calculamos los bordes de la pantalla
        float worldLeft = camX - Parameters::screenWidth * 0.5f;
        float worldRight = camX + Parameters::screenWidth * 0.5f;
        float worldTop = camY - Parameters::screenHeight * 0.75f;
        float worldBottom = camY + Parameters::screenHeight * 0.25f;

        // Calculamos la zona de spawn superior, algo fuera de la pantalla
        Vector2 spawnTop(
            camX + (rand() % Parameters::screenWidth) - Parameters::screenWidth * 0.5f,
            worldTop + 100
        );

        // Calculamos la zona de spawn izquierda, algo fuera de la pantalla
        Vector2 spawnLeft(
            worldLeft,
            camY - (rand() % 400)
        );

        // Calculamos la zona de spawn derecha, algo fuera de la pantalla
        Vector2 spawnRight(
            worldRight,
            camY - (rand() % 400)
        );

        // Elegimos una de las tres zonas aleatoriamente
        int side = rand() % 3;
        Vector2 spawnPos =
            (side == 0) ? spawnTop :
            (side == 1) ? spawnLeft :
            spawnRight;

        // Definimos las probabilidades de cada tipo de enemigo
        float probShooter = 0.5f;   // 50%
        float probGrenadier = 0.3f; // 30%
        float probHybrid = 0.2f;    // 20%

        int r = rand() % 100;

        // Creamos el tipo de enemigo que toque
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