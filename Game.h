#pragma once
#include <map>
#include "Scene.h"
#include "Camera.h"
#include "Player.h"
#include "GameScene.h"

/// <summary>
/// Singleton que controla el ciclo de vida del juego
/// </summary>
class Game
{
public:
	static void Create();							// Inicializa el Singleton
	static void Destroy();							// Libera memoria
	static void Play();								// Entra en el loop

	static std::map<int, bool> keyDown;				// Sistema de inouts global

	static Camera camera;							// Camara global
	static int currentScore;						// Puntuacion actual
	
	static int LoadHighScore();						// Carga high score
	static void SaveHighScore(int score);			// Guarda high score

	static void ChangeScene(Scene* newScene);		// Cambia de escena
	static void SavePlayerState(Player* p);			// Guarda el estado del jugador
	static void RestorePlayerState(GameScene* gs);	// Restaura el estado del jugador
	static void OnPlayerDeath(Player* p);			// Gestiona lo que ocurre cuando muere el jugador
	static void OnGameOver();						// Gestiona lo que ocurre cuando pierde el jugador
	static Game* instance;							// Referencia Singleton
	Scene* currentScene;							// Escena actual

	static bool newHS;								// Indica si hay un nuevo high score o no
	static int nextExtraLifeScore;					// Puntos necesarios para vida extra

private:
	static float DeltaTime;							// Tiempo de ejecucion global

	class GraphicsInterface* GI;					// Puntero a GraphicsInterface
	class MyPhysics* mph;							// Puntero a MyPhysics

	bool game_end = false;							// Indica si el juego ha terminado o no

	Game();											// Constructor
	~Game();										// Destructor

	void Loop();									// Loop principal del juego
	void Update(float deltaTime);					// Controla logica del juego
	void Render();									// Controla la renderizacion del juego
	void UpdateInputs();							// Controla los inputs introducidos

	static bool pendingRespawn;						// Indica si se debe respawnear
	static int pendingLives;						// Numero de vidas del jugador
	static int pendingGrenades;						// Numero de granadas del jugador
	static bool pendingGameOver;					// Indica si se debe lanzar gameover
	static int pendingArea;							// Numero de area en la que se encuentra el jugador
};
