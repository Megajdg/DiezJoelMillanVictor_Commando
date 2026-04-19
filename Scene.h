#pragma once
#include <vector>

/// <summary>
/// Define la clase escena, de aqui derivan todas las escenas
/// </summary>
class Scene
{
	class Sprite* debugCircle256 = NULL;
	class Sprite* debugRectangle400 = NULL;

protected:

	//Lista de actores del mundo
	std::vector<class Actor*> actors;

	// Lista de actores de interfaz
	std::vector<class Actor*> ui;

	// Lista de actores pendientes de eliminar
	std::vector<class Actor*> actors_to_destroy;

	// Color de fondo de la escena
	struct {
		unsigned char r, g, b;
	} backgroundColor;

public:
	// Acceso a la interfaz grafica y al motor de fisicas
	class GraphicsInterface* GI;
	class MyPhysics* mph;

	// Devuelve la lista de actores
	const std::vector<Actor*>& GetActors() const { return actors; }

public:
	// Constructor y estructor
	Scene(class GraphicsInterface* GI, class MyPhysics* mph);
	~Scene();

	virtual void Update(float deltaTime);

	virtual void Render();

	// Añadir actores al mundo o a la UI
	void AddActor(Actor* a);
	void AddUI(Actor* a);

	// Marcar actor para destruir y eliminarlo al final del frame
	void DestroyActor(class Actor*);
	void RemoveActors();
};