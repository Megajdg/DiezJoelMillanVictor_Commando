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
#include "AudioManager.h"

// VARIABLES ESTATICAS GLOBALES

Game* Game::instance = 0;				// Patron Singleton
std::map<int, bool> Game::keyDown;		// Input global
float Game::DeltaTime = 0.0f;			// Tiempo de ejecucion
Camera Game::camera(Vector2(0.f));		// Camara global
bool Game::pendingRespawn = false;		// Controla si se debe hacer respawn
int Game::pendingLives = 0;				// Guarda las vidas a trasladar a otra escena
int Game::pendingGrenades = 0;			// Guarda las granadas a trasladar a otra escena
bool Game::pendingGameOver = false;		// Controla si se debe hacer gameover
int Game::currentScore = 0;				// Guarda la puntuacion a trasladar a otra escena
int Game::pendingArea = 1;				// Area en la cual hacer respawn
bool Game::newHS = false;				// Define si hay un nuevo high score
int Game::nextExtraLifeScore = 10000;	// Puntos para obtener una vida extra

void Game::Create()
{
	// Inicializamos el Singleton
	if (!instance)
		instance = new Game();

	// Situamos la camara en la posicion inicial del nivel
	Game::camera.position = Parameters::initial_camera_position;
}

void Game::Destroy()
{
	// Limpieza del Singleton
	if (instance)
		delete instance;
}

void Game::Play()
{
	// Entramos en el loop de juego
	if (instance)
		instance->Loop();
}

void Game::ChangeScene(Scene* newScene)
{
	// Si la escena actual es una escena de juego, borramos los colliders del mapa
	if (instance->currentScene)
	{
		GameScene* gs = dynamic_cast<GameScene*>(instance->currentScene);
		if (gs && gs->mapActor)
			instance->mph->RemoveMapColliders(gs->mapActor);
	}

	// Limpiamos la escena actual
	if (instance->currentScene)
		delete instance->currentScene;

	instance->currentScene = newScene;

	// Resetear inputs para evitar errores
	Game::keyDown.clear();

	// Si no es null, cerramos el juego
	if (!newScene)
		instance->game_end = true;
}

void Game::SavePlayerState(Player* p)
{
	// Guardamos la informacion esencial del jugador (vidas, granadas y area actual)
	pendingLives = p->lives;
	pendingGrenades = p->grenades;
	GameScene* gs = (GameScene*)Game::instance->currentScene;
	pendingArea = gs->area;
}

void Game::RestorePlayerState(GameScene* gs)
{
	// Cargamos la informacion esencial del jugador (vidas, granadas y puntuacion)
	gs->player->lives = pendingLives;
	gs->player->grenades = pendingGrenades;
	gs->score = currentScore;
}

void Game::OnPlayerDeath(Player* p)
{
	// Marcamos que el jugador debe hacer respawn y guardamos su informacion
	pendingRespawn = true;
	SavePlayerState(p);
}

void Game::OnGameOver()
{
	// Marcamos que el jugador debe hacer gameover
	pendingGameOver = true;
}

Game::Game()
{
	// Inicializamos graficos, fisicas y audio
	GI = new GraphicsInterface();
	mph = new MyPhysics();
	AudioManager::instance().Init();

	// Entramos al menu principal
	currentScene = new MenuScene(GI, mph);
}

Game::~Game()
{
	// Limpiamos todo
	delete GI;
	delete mph;
	AudioManager::instance().Close();
}

void Game::UpdateInputs()
{
	// Capturamos un evento SDL
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_EVENT_QUIT:
			// Si el evento es de salida, marcamos fin de juego
			game_end = true;
			break;
		case SDL_EVENT_KEY_DOWN:
			// Si el evento es de pulsar una tecla, la marcamos como true, salvo el espacio, que solo la marca 1 vez
			keyDown[event.key.key] = true;
			if (event.key.key == SDLK_SPACE && event.key.repeat == 0)
				keyDown[SDLK_SPACE] = true;
			else
				keyDown[SDLK_SPACE] = false;
			break;
		case SDL_EVENT_KEY_UP:
			// Si el evento es de levantar una tecla, la marcamos como false
			keyDown[event.key.key] = false;
			break;
		}
	}
}

int Game::LoadHighScore()
{
	// Cargamos el highscore del juego
	std::ifstream f("highscore.txt");
	int hs = 0;
	if (f.is_open())
		f >> hs;
	return hs;
}

void Game::SaveHighScore(int score)
{
	// Guardamos el highscore del juego
	newHS = true;
	std::ofstream f("highscore.txt");
	if (f.is_open())
		f << score;
}

void Game::Update(float deltaTime)
{
	// Actualizamos inputs, logica y camara
	UpdateInputs();
	currentScene->Update(deltaTime);
	Game::camera.Update();
}

void Game::Render()
{
	// Renderizamos la escena
	currentScene->Render();
}

void Game::Loop()
{
	// Medimos tiempo entre frames y fijamos los frames deseados
	unsigned int millis = SDL_GetTicks();
	float desired_deltatime = 1.0f / Parameters::desired_FPS;

	// Mientras el juego no este maracado para finalizar
	while (!game_end)
	{
		// Actualizamos logica
		Update(DeltaTime);

		// Actualizamos fisica
		mph->Update();

		// Eliminamos actores que deben ser destruidos
		currentScene->RemoveActors();

		// Si se debe respawnear, recargamos la escena con el area actual, y restauramos la informacion esencial del jugador
		if (pendingRespawn)
		{
			ChangeScene(new GameScene(instance->GI, instance->mph, pendingArea));

			GameScene* gs = (GameScene*)instance->currentScene;
			RestorePlayerState(gs);

			pendingRespawn = false;
		}

		// Si se debe hacer gameover, guardamos el high score, y cambiamos la escena a la de gameover
		if (pendingGameOver)
		{
			int hs = LoadHighScore();
			if (currentScore > hs)
				SaveHighScore(currentScore);

			ChangeScene(new GameOverScene(instance->GI, instance->mph));
			pendingGameOver = false;
		}

		// Renderizamos
		Render();

		// Calculamos el deltatime real
		DeltaTime = (SDL_GetTicks() - millis)/1000.f;
		millis = SDL_GetTicks();

		// Y hacemos un sleep para mantener los frames
		Sleep(int(abs(desired_deltatime - DeltaTime)/1000));
	}
}