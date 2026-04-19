#pragma once
#include "Scene.h"
#include "Player.h"
#include "StaticMapActor.h"
#include <vector>

/// <summary>
/// Escena de juego
/// </summary>
class GameScene : public Scene
{
public:
    GameScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber);           // Constructor

    StaticMapActor* mapActor = nullptr;                                         // Puntero al actor de colliders del mapa

    void Update(float dt) override;                                             // Gestiona logica de volver, de vidas extra y de deteccion de final de area
    void Render() override;                                                     // Gestiona el renderizado

    int score = 0;                                                              // Puntuacion local
    int area = 1;                                                               // Numero de area actual

    void AddScore(int amount);                                                  // Actualiza la puntuacion
    void GenerateColliders(std::string mapImage, std::string collisionMask);    // Genera las colisiones de los mapas

    Player* GetPlayer() const { return player; }                                // Devuelve al jugador

    Player* player = nullptr;                                                   // Puntero directo al jugador
    std::vector<Vector2> grenadePickupPositions;                                // Vector de posiciones de powerup
};
