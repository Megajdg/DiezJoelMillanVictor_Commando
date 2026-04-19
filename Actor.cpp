#include "Actor.h"
#include "Scene.h"

// Guardamos un puntero a la escena que contiene el actor
Actor::Actor(Scene* myScene)
{
	this->myScene = myScene;
}

void Actor::Update(float deltaTime)
{
}

void Actor::Render()
{
}
