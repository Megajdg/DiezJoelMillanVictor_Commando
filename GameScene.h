#pragma once
#include "Scene.h"
#include "Player.h"
#include "StaticMapActor.h"
#include <vector>

class GameScene : public Scene
{
public:
    GameScene(GraphicsInterface* GI, MyPhysics* mph, int areaNumber);

    StaticMapActor* mapActor = nullptr;

    void Update(float dt) override;
    void Render() override;

    int score = 0;

    int area = 1;

    void AddScore(int amount);

    void GenerateColliders(std::string mapImage, std::string collisionMask);

    Player* GetPlayer() const { return player; }

    Player* player = nullptr;

    std::vector<Vector2> grenadePickupPositions;

    bool firstFrame = true;
};
