#pragma once
#include "Scene.h"
#include "Player.h"
#include "StaticMapActor.h"

class GameScene : public Scene
{
public:
    GameScene(GraphicsInterface* GI, MyPhysics* mph);

    StaticMapActor* mapActor = new StaticMapActor(this);

    void Update(float dt) override;
    void Render() override;

    int score = 0;

    void AddScore(int amount);

    void GenerateColliders();

    Player* GetPlayer() const { return player; }

    Player* player = nullptr;
};
