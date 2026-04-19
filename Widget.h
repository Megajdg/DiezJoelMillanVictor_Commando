#pragma once
#include "Actor.h"

/// <summary>
/// Define widget que se usa para dibujar en pantalla sin fisicas
/// </summary>
class Widget : public Actor
{
public:
	// Constructor del widget, una actor que se dibuja en pantalla, no participa en fisicas ni logica
	Widget(class Scene* myScene);

	virtual void Update(float deltaTime);
	virtual void Render();
};