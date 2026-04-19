#include "Scene.h"
#include "Actor.h"
#include "GraphicsInterface.h"
#include "MyPhysics.h"

#ifdef DEBUG_COLLIDERS
#include "Sprite.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#endif

// Constructor de la escena, inicializa color de fondo, guarda punteros a GI y MyPhysics
Scene::Scene(GraphicsInterface* GI, MyPhysics* mph)
{
	backgroundColor.b = backgroundColor.g = backgroundColor.r = 0;
	this->GI = GI;
	this->mph = mph;

#ifdef DEBUG_COLLIDERS
	GI->LoadImage("circle_PNG256_256.png");
	debugCircle256 = new Sprite(this, "circle_PNG256_256.png", 1);
	GI->LoadImage("square_PNG400_400.png");
	debugRectangle400 = new Sprite(this, "square_PNG400_400.png", 1);
#endif // DEBUG_COLLIDERS
}

// Destructor de la escena y elimina todos los actores y a sus colliders asociados
Scene::~Scene()
{
	for (Actor* actor : actors)
	{
		mph->RemoveActor(actor);	// Quita colliders del motor de fisicas

		delete actor;				// Elimina al actor para liberar memoria
	}
}

// Update general de la escena, llama al Update de todos los actores y luego elimina los marcados para destruir
void Scene::Update(float deltaTime)
{
	for (size_t i = 0; i < actors.size(); ++i)
	{
		actors[i]->Update(deltaTime);
	}

	RemoveActors();
}

// Render general de la escena, dibuja fondo, actores, UI y opcionalmente colliders de debug
void Scene::Render()
{
	GI->ClearScreen(backgroundColor.r, backgroundColor.g, backgroundColor.b);

	// Dibujar actores del mundo
	for (size_t i = 0; i < actors.size(); ++i)
		actors[i]->Render();

	// Dibujar elementos de interfaz
	for (size_t i = 0; i < ui.size(); ++i)
		ui[i]->Render();

#ifdef DEBUG_COLLIDERS

	for (auto pair : mph->GetCollidersByActor())
	{
		Actor* act = pair.first;
		for (Collider* col : *pair.second)
		{
			Transform t;
			if (dynamic_cast<CircleCollider*>(col))
			{
				t.position = act->transform.position + col->relative_position.rotate(act->transform.rotation);
				t.scale = ((CircleCollider*)col)->radius * 2;
				debugCircle256->transform = t;
				debugCircle256->Render();
			}
			else
			{
				t.position = act->transform.position + col->relative_position;
				t.scale = ((RectangleCollider*)col)->size;
				debugRectangle400->transform = t;
				debugRectangle400->Render();
			}
		}
	}

#endif
	// Renderiza
	GI->DrawFrame();
}

// Añade un actor a la escena 
void Scene::AddActor(Actor* a)
{
	if (std::find(actors.begin(), actors.end(), a) != actors.end())
	{
		std::cout << "WARNING: AddActor duplicado: " << a << "\n";
		return;
	}

	actors.push_back(a);
}

// Añade un actor a la lista de UI
void Scene::AddUI(Actor* a)
{
	if (std::find(ui.begin(), ui.end(), a) != ui.end())
	{
		std::cout << "WARNING: AddUI duplicado: " << a << "\n";
		return;
	}

	ui.push_back(a);
}

// Marca un actor para ser destruido al final del frame
void Scene::DestroyActor(Actor* act)
{
	if (!act) return;

	if (std::find(actors_to_destroy.begin(), actors_to_destroy.end(), act) != actors_to_destroy.end())
		return;

	actors_to_destroy.push_back(act);
}

// Elimina definitivamente los actores marcador para destruir
void Scene::RemoveActors()
{
	for (Actor* act : actors_to_destroy)
	{
		// Quita de la lista a los actores
		auto it = std::find(actors.begin(), actors.end(), act);
		if (it != actors.end()) {
			actors.erase(it);
		}

		// Quita colliders del motor de fisicas
		mph->RemoveActor(act);

		// Libera memoria
		delete act;
	}

	// Limpia el vector
	actors_to_destroy.clear();
}