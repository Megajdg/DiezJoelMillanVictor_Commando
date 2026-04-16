#include "Scene.h"
#include "Actor.h"

#include "GraphicsInterface.h"
#include "MyPhysics.h"

#ifdef DEBUG_COLLIDERS
#include "Sprite.h"
#include "CircleCollider.h"
#include "RectangleCollider.h"
#endif

Scene::Scene(GraphicsInterface* GI, MyPhysics * mph)
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

Scene::~Scene()
{
	for (Actor* actor : actors)
	{
		mph->RemoveActor(actor);

		delete actor;
	}
}

void Scene::Update(float deltaTime)
{
	for (size_t i = 0; i < actors.size(); ++i)
    {
        actors[i]->Update(deltaTime);
    }

	RemoveActors();
}

void Scene::Render()
{
	GI->ClearScreen(backgroundColor.r, backgroundColor.g, backgroundColor.b);

	for (size_t i = 0; i < actors.size(); ++i)
		actors[i]->Render();

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
	GI->DrawFrame();
}

void Scene::AddActor(Actor* a)
{
	if (std::find(actors.begin(), actors.end(), a) != actors.end())
	{
		std::cout << "WARNING: AddActor duplicado: " << a << "\n";
		return;
	}

	actors.push_back(a);
}

void Scene::AddUI(Actor* a)
{
	if (std::find(ui.begin(), ui.end(), a) != ui.end())
	{
		std::cout << "WARNING: AddUI duplicado: " << a << "\n";
		return;
	}

	ui.push_back(a);
}

void Scene::DestroyActor(Actor* act)
{
	if (!act) return;

	if (std::find(actors_to_destroy.begin(), actors_to_destroy.end(), act) != actors_to_destroy.end())
		return;

	actors_to_destroy.push_back(act);
}

void Scene::RemoveActors()
{
	for (Actor* act : actors_to_destroy)
	{
		auto it = std::find(actors.begin(), actors.end(), act);
		if (it != actors.end()) {
			actors.erase(it);
		}

		mph->RemoveActor(act);

		delete act;
	}

	actors_to_destroy.clear();
}
