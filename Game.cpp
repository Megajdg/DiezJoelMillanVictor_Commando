#include "Game.h"
#include "GraphicsInterface.h"
#include "MyPhysics.h"
#include "SDL3/SDL.h"
#include "Parameters.h"
#include <Windows.h>
#include "IntroScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include <fstream>
#include "GameOverScene.h"

Game* Game::instance = 0;
std::map<int, bool> Game::keyDown;
float Game::DeltaTime = 0.0f;
Camera Game::camera(Vector2(0.f));
bool Game::pendingRespawn = false;
int Game::pendingLives = 0;
int Game::pendingGrenades = 0;
bool Game::pendingGameOver = false;
int Game::currentScore = 0;
int Game::pendingArea = 1;

void Game::Create()
{
	if (!instance)
		instance = new Game();

	Game::camera.position = Parameters::initial_camera_position;
}

void Game::Destroy()
{
	if (instance)
		delete instance;
}

void Game::Play()
{
	if (instance)
		instance->Loop();
}

void Game::ChangeScene(Scene* newScene)
{
	if (instance->currentScene)
	{
		GameScene* gs = dynamic_cast<GameScene*>(instance->currentScene);
		if (gs && gs->mapActor)
			instance->mph->RemoveMapColliders(gs->mapActor);
	}

	if (instance->currentScene)
		delete instance->currentScene;

	instance->currentScene = newScene;

	// Resetear inputs para evitar flancos heredados
	Game::keyDown.clear();

	if (!newScene)
		instance->game_end = true;
}

void Game::SavePlayerState(Player* p)
{
	pendingLives = p->lives;
	pendingGrenades = p->grenades;
	GameScene* gs = (GameScene*)Game::instance->currentScene;
	pendingArea = gs->area;
}

void Game::RestorePlayerState(GameScene* gs)
{
	gs->player->lives = pendingLives;
	gs->player->grenades = pendingGrenades;
	gs->score = currentScore;
}

void Game::OnPlayerDeath(Player* p)
{
	pendingRespawn = true;
	SavePlayerState(p);
}

void Game::OnGameOver()
{
	pendingGameOver = true;
}

Game::Game()
{
	GI = new GraphicsInterface();
	mph = new MyPhysics();

	currentScene = new MenuScene(GI, mph);
}

Game::~Game()
{
	delete GI;
	delete mph;
}

void Game::UpdateInputs()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			game_end = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			keyDown[event.key.key] = true;
			if (event.key.key == SDLK_SPACE && event.key.repeat == 0)
			{
				keyDown[SDLK_SPACE] = true;
			}
			else
				keyDown[SDLK_SPACE] = false;
			break;
		case SDL_EVENT_KEY_UP:
			keyDown[event.key.key] = false;
			break;
		}
	}
}

int Game::LoadHighScore()
{
	std::ifstream f("highscore.txt");
	int hs = 0;
	if (f.is_open())
		f >> hs;
	return hs;
}

void Game::SaveHighScore(int score)
{
	std::ofstream f("highscore.txt");
	if (f.is_open())
		f << score;
}

void Game::Update(float deltaTime)
{
	UpdateInputs();
	currentScene->Update(deltaTime);
	Game::camera.Update();
}

void Game::Render()
{
	currentScene->Render();
}

void Game::Loop()
{
	unsigned int millis = SDL_GetTicks();
	float desired_deltatime = 1.0f / Parameters::desired_FPS;
	while (!game_end)
	{
		Update(DeltaTime);

		mph->Update();

		currentScene->RemoveActors();

		if (pendingRespawn)
		{
			ChangeScene(new GameScene(instance->GI, instance->mph, pendingArea));

			GameScene* gs = (GameScene*)instance->currentScene;
			RestorePlayerState(gs);

			pendingRespawn = false;
		}

		if (pendingGameOver)
		{
			int hs = LoadHighScore();
			if (currentScore > hs)
				SaveHighScore(currentScore);

			ChangeScene(new GameOverScene(instance->GI, instance->mph));
			pendingGameOver = false;
		}

		Render();

		DeltaTime = (SDL_GetTicks() - millis)/1000.f;
		millis = SDL_GetTicks();

		Sleep(int(abs(desired_deltatime - DeltaTime)/1000));
	}
}
