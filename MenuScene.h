#pragma once
#include "scene.h"

class MenuScene : public Scene
{
public:
	MenuScene(GraphicsInterface* GI, MyPhysics* mph);

	void Update(float dt) override;
	void Render() override;

private:
	int selected = 0; // 0 -> Jugar, 1 -> Ayuda, 2 -> Créditos, 3 -> Salir
};

