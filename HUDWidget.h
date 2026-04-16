#pragma once
#include "Widget.h"
#include "Player.h"

class HUDWidget : public Widget
{
public:
    Player* player = nullptr;
    class GameScene* gameScene = nullptr;

    HUDWidget(Scene* scene, Player* player);
    void Render() override;
};
