#pragma once

#include <map>
#include "Scene.h"
#include "Camera.h"
#include "Player.h"

class Game
{
public:
	static void Create();
	static void Destroy();

	static void Play();

	static std::map<int, bool> keyDown;

	static Camera camera;
	static int currentScore;
	
	static int LoadHighScore();

	static void ChangeScene(Scene* newScene);
	static void OnPlayerDeath(Player* p);
	static void OnGameOver();
private:
	static Game* instance;
	
	static float DeltaTime;

	class GraphicsInterface* GI;
	class MyPhysics* mph;

	bool game_end = false;

	Scene* currentScene;

	Game();
	~Game();

	void Loop();
	void Update(float deltaTime);
	void Render();
	void UpdateInputs();
	void SaveHighScore(int score);

	static bool pendingRespawn;
	static int pendingLives;
	static int pendingGrenades;
	static bool pendingGameOver;
};

