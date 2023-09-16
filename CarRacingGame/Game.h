#pragma once
#include <iostream>
#include <thread>
#include <fstream>

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Player.h"
#include "ConfigManager.h"
#include "WindowConfig.h"
#include "Scenes.h"
#include "Timer.h"
#include "Enemy.h"

using namespace sf;
using namespace std;

class Game
{
public:
	Game(string configPath);
	~Game();

	void run();

	inline void setCurrentScene(int index)
	{
		int oldIndex = this->sceneIndex;
		this->sceneIndex = index;
		this->onSceneChanged(oldIndex, this->sceneIndex);
	};

	inline float getSpeedMultiplier() { return this->speedMultiplier; }

public:
	//multithreaded methods
	void spawnEnemy();

	void difficultyUpdate();

private:
	void initWindow();

	void initGame();

	void initRender();
	//logic part of each scene
	void updateSceneHandler();

	//rendering part of each scene
	void renderSceneHandler();

	void updateSFMLEvents();

	void onSceneChanged(int oldScene, int newScene);

	void update();

	void render();

	void endGame();
private:
	RenderWindow* window;
	Event sfEvent;

	WindowConfig windowConfig;
	string configPath;

	Texture bgRoadSpriteTex, bgMainMenuTex, fittingTex;
	Sprite bgRoadSprite, bgRoadSprite1, bgMainMenu, fittingSkin;

	//speed is 'your car speed' for distance calculating; speedMultiplier is like difficulty coefficient
	float offsetY, speedMultiplier, speed, distance, points;
	int elapsedTime, gameStartedTime, maxEnemyCount;

	Font textFont;
	Text menuTitleText, timerText, distanceText, pointsText, pressToStartText, bestDistanceText, bestPointsText, fittingDecoText;

	Timer startTimer;

	Clock clock, gameClock;

	Player* player;

	vector<Enemy*> enemies;

	volatile bool running, routineStarted;

	int sceneIndex, skinIndex, skinCount;

	Music music;
};

