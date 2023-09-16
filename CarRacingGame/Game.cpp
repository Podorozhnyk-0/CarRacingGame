#include "Game.h"

Game::Game(string configPath)
{
	this->running = false;
	this->configPath = configPath;

	ifstream file(this->configPath);
	
	if (!file.is_open() || file.peek() == EOF)
	{
		ofstream writeFile(this->configPath);
		writeFile.close();

		ConfigManager::saveProperty(this->configPath, "VideoMode", "1280,720");
		ConfigManager::saveProperty(this->configPath, "Title", "Car Racing Game");
		ConfigManager::saveProperty(this->configPath, "Style", "4");
		ConfigManager::saveProperty(this->configPath, "FPS Limit", "60");
		ConfigManager::saveProperty(this->configPath, "bgMainMenuPath", "Images/background.jpg");
		ConfigManager::saveProperty(this->configPath, "bgRoadSpritePath", "Images/Road.jpg");
		ConfigManager::saveProperty(this->configPath, "textFontPath", "Fonts/PublicPixel.TTF");
		ConfigManager::saveProperty(this->configPath, "playerCarsPath", "Images/PlayerCars/0.PNG,Images/PlayerCars/1.PNG/Images/PlayerCars/2.PNG");
		ConfigManager::saveProperty(this->configPath, "currentCarPath", "Images/PlayerCars/0.PNG");
		ConfigManager::saveProperty(this->configPath, "enemyCarsPath", "Images/EnemyCars/0.PNG");
		ConfigManager::saveProperty(this->configPath, "music", "Audio/8BitAdventure.wav,Audio/RetroPlatforming.wav");
		ConfigManager::saveProperty(this->configPath, "bestStats", "0,0");
	}

	this->initWindow();
	this->setCurrentScene(MAIN_MENU);
}

Game::~Game()
{
	delete this->window;
	delete this->player;
}

void Game::run()
{
	initGame();
	initRender();

	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}

void Game::spawnEnemy()
{
	while (this->running)
	{
		vector<Vector2f> pointsEnemy =
		{
			Vector2f(270.f, -200),
			Vector2f(510.f, -200),
			Vector2f(740.f, -200),
			Vector2f(970.f, -200)
		};

		int enemyCount = rand() % this->maxEnemyCount;
		for (size_t i = 0; i <= enemyCount; i++)
		{
			auto enemy = Enemy::create(this->configPath, this->window, &this->speedMultiplier, pointsEnemy);
			this->enemies.push_back(enemy);

			int delay = (this->speedMultiplier < 7) ? 2 : 1;
			sleep(seconds(delay));
		}
		
		int delay = (this->speedMultiplier < 2.5f)? 4 : 1.5f / this->speedMultiplier;

		sleep(seconds(delay));
	}
}

void Game::difficultyUpdate()
{
	while (this->running)
	{
		sleep(seconds(20));

		if (this->speedMultiplier > 20) return;
		this->speedMultiplier += 0.45f;
	}
}

void Game::endGame()
{
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		delete this->enemies[i];
	}
	this->enemies.clear();

	int prevDistance = atoi(ConfigManager::loadProperty(this->configPath, "bestStats", ',')[0].c_str());
	int prevPoints = atoi(ConfigManager::loadProperty(this->configPath, "bestStats", ',')[1].c_str());

	if (this->distance > prevDistance || this->points > prevPoints)
	{
		ConfigManager::saveProperty(this->configPath, "bestStats",
			to_string((int)roundf(this->distance)) + "," + to_string((int)roundf(this->points)));
	}
	this->setCurrentScene(MAIN_MENU);
	this->running = false;
}

void Game::initWindow()
{
	if (!this->configPath.empty())
	{
		this->windowConfig = WindowConfig();

		vector<string> videoModeRaw = ConfigManager::loadProperty(this->configPath, "VideoMode", ',');

		this->windowConfig.mode = VideoMode(atoi(videoModeRaw[0].c_str()), atoi(videoModeRaw[1].c_str()));
		this->windowConfig.title = ConfigManager::loadProperty(this->configPath, "Title")[0];
		this->windowConfig.style = atoi(ConfigManager::loadProperty(this->configPath, "Style")[0].c_str());
		this->windowConfig.fpsLimit = atoi(ConfigManager::loadProperty(this->configPath, "FPS Limit")[0].c_str());

		this->window = new RenderWindow(this->windowConfig.mode, this->windowConfig.title, this->windowConfig.style);
		this->window->setFramerateLimit(this->windowConfig.fpsLimit);
	}
	else
	{
		this->window = new RenderWindow(VideoMode(800, 600), "None");
		this->window->setFramerateLimit(60);
	}
}

void Game::initGame()
{
	this->startTimer = Timer();
	this->startTimer.setEndTime(5000.f);

	this->speedMultiplier = 1.f;
	this->maxEnemyCount = 3;
	this->speed = 60.f;

	vector<Vector2f> points = vector<Vector2f>(4);
	points[0] = Vector2f(250.f, 530.f);
	points[1] = Vector2f(480.f, 530.f);
	points[2] = Vector2f(710.f, 530.f);
	points[3] = Vector2f(930.f, 530.f);

	this->skinCount = ConfigManager::loadProperty(this->configPath, "playerCarsPath", ',').size();
	this->player = new Player(this->configPath, this->window, points);
}

void Game::initRender()
{
	string texturePath = ConfigManager::loadProperty(this->configPath, "bgMainMenuPath")[0];

	this->bgMainMenuTex.loadFromFile(texturePath);
	this->bgMainMenu = Sprite(this->bgMainMenuTex, IntRect(0, 0, this->window->getSize().x, this->window->getSize().y));

	texturePath = ConfigManager::loadProperty(this->configPath, "bgRoadSpritePath")[0];

	this->bgRoadSpriteTex.loadFromFile(texturePath);
	this->bgRoadSpriteTex.setRepeated(true);

	this->bgRoadSprite = Sprite(this->bgRoadSpriteTex, IntRect(-115, this->offsetY, this->window->getSize().x, this->window->getSize().y));
	this->bgRoadSprite.setPosition(0, 0);

	string fontPath = ConfigManager::loadProperty(this->configPath, "textFontPath")[0];
	this->textFont.loadFromFile(fontPath);

	this->menuTitleText = Text("CAR RACING GAME", this->textFont, 50U);
	this->menuTitleText.setPosition(Vector2f(this->window->getSize().x / 2 - this->menuTitleText.getLocalBounds().width / 2, 40));

	this->pressToStartText = Text("Press [SPACE] to start", this->textFont, 30U);
	this->pressToStartText.setPosition(Vector2f(this->window->getSize().x / 2 - this->pressToStartText.getLocalBounds().width / 2, 
		this->window->getSize().y / 2 - this->pressToStartText.getLocalBounds().height / 2 + 100));

	this->distanceText = Text("0m", this->textFont, 50U);
	this->pointsText = Text("Score: 0", this->textFont, 40U);

	this->pointsText.setPosition(Vector2f(0, this->distanceText.getLocalBounds().height + 10));

	this->fittingDecoText = Text("<    >", this->textFont, 180U);
	this->fittingDecoText.setPosition(this->window->getSize().x / 2 - this->fittingDecoText.getLocalBounds().width / 2,
		this->window->getSize().y / 2 - this->fittingDecoText.getLocalBounds().height / 2 - 50);
}

void Game::updateSceneHandler()
{
	switch (this->sceneIndex)
	{
	case MAIN_MENU:
		break;
	case FITTING_ROOM:
		break;
	case GAME:
		this->running = !this->startTimer.isRunning();
		this->startTimer.update();

		if (this->running)
		{
			this->elapsedTime = roundf(this->gameClock.getElapsedTime().asSeconds() - this->gameStartedTime);

			this->speed = 60.f * this->speedMultiplier;
			this->distance += 1 * this->speed / 60.f / 12.5f;
			this->points = this->distance / 3;

			if (!this->routineStarted)
			{
				thread enemyThread = thread(&Game::spawnEnemy, this);
				enemyThread.detach();

				thread difficultyThread = thread(&Game::difficultyUpdate, this);
				difficultyThread.detach();
				this->routineStarted = true;
			}

			for (auto it = this->enemies.begin(); it < this->enemies.end();)
			{
				//enemies update loop
				if ((*it)->getPosition().y > this->window->getSize().y + 200)
				{
					delete (*it);
					it = enemies.erase(it);
					continue;
				}
				(*it)->update();

				Sprite sprite = (*it)->getSprite();
				if (this->player->checkCollision(sprite))
				{
					endGame();
					break;
				}

				it++;
			}
		}
		break;
	}
}

void Game::renderSceneHandler()
{
	switch (this->sceneIndex)
	{
	case MAIN_MENU:
	{
		this->bestPointsText.setPosition(Vector2f(0, this->window->getSize().y - this->bestPointsText.getLocalBounds().height - 10));
		this->bestDistanceText.setPosition(Vector2f(0, this->window->getSize().y - this->bestPointsText.getLocalBounds().height * 3));

		this->window->draw(this->bgMainMenu);
		this->window->draw(this->menuTitleText);
		this->window->draw(this->pressToStartText);
		this->window->draw(this->bestDistanceText);
		this->window->draw(this->bestPointsText);
	}
	break;
	case FITTING_ROOM:
		this->window->draw(this->fittingDecoText);
		this->window->draw(this->fittingSkin);
		break;
	case GAME:
	{
		this->timerText = Text(to_string((int)roundf(this->startTimer.getLeftSeconds())), this->textFont, 60U);
		this->timerText.setPosition(Vector2f(this->window->getSize().x / 2 - this->timerText.getLocalBounds().width / 2, 40));

		this->distanceText = Text(to_string((int)roundf(this->distance)) + "m", this->textFont, 50U);

		this->pointsText = Text("Score: " + to_string((int)roundf(this->points)), this->textFont, 40U);
		this->pointsText.setPosition(Vector2f(0, this->distanceText.getLocalBounds().height + 10));

		float time = this->clock.getElapsedTime().asMicroseconds() / 6000;

		if (offsetY < this->window->getSize().y)
		{
			offsetY -= speedMultiplier * time;
		}
		else
		{
			offsetY = 0;
		}

		bgRoadSprite.setTextureRect(IntRect(-115, this->offsetY, this->window->getSize().x, this->window->getSize().y));
		clock.restart();


		this->window->draw(this->bgRoadSprite);
		this->window->draw(*this->player);
		for (size_t i = 0; i < enemies.size(); i++)
		{
			this->window->draw(*enemies[i]);
		}
		if (!this->running)
			this->window->draw(this->timerText);
		this->window->draw(this->distanceText);
		this->window->draw(this->pointsText);
	}
	break;
	}
}

void Game::updateSFMLEvents()
{
	while (this->window->pollEvent(this->sfEvent))
	{
		if (this->sfEvent.type == Event::Closed)
			this->window->close();

		switch (this->sceneIndex)
		{
		case MAIN_MENU:
			switch (this->sfEvent.type)
			{
			case Event::KeyPressed:
				if (this->sfEvent.key.code == Keyboard::Key::Space)
				{
					this->setCurrentScene(GAME);
				}
				else if (this->sfEvent.key.code == Keyboard::Key::F)
				{
					this->setCurrentScene(FITTING_ROOM);
				}
				break;
			}
			break;
		case FITTING_ROOM:
			switch (this->sfEvent.type)
			{
			case Event::KeyPressed:
				if (this->sfEvent.key.code == Keyboard::Key::Escape)
				{
					this->setCurrentScene(MAIN_MENU);
					break;
				}
				string path;
				if (this->sfEvent.key.code == Keyboard::Key::Right)
				{
					if (this->skinIndex >= this->skinCount - 1) break;
					path = ConfigManager::loadProperty(this->configPath, "playerCarsPath", ',')[++this->skinIndex];
					this->fittingTex.loadFromFile(path);
				}
				if (this->sfEvent.key.code == Keyboard::Key::Left)
				{
					if (this->skinIndex < 1) break;
					path = ConfigManager::loadProperty(this->configPath, "playerCarsPath", ',')[--this->skinIndex];
					this->fittingTex.loadFromFile(path);
				}
				ConfigManager::saveProperty(this->configPath, "currentCarPath", path);
				break;
			}
			break;
		case GAME:
			switch (this->sfEvent.type)
			{
			case Event::KeyPressed:
				if (this->running)
					this->player->move(this->sfEvent.key.code);

				break;
			}
			break;
		}
	}
}

void Game::update()
{
	this->updateSFMLEvents();
	this->updateSceneHandler();
}

void Game::render()
{
	this->window->clear();
	this->renderSceneHandler();
	this->window->display();
}

void Game::onSceneChanged(int oldScene, int newScene)
{
	auto musicTracks = ConfigManager::loadProperty(this->configPath, "music", ',');
	this->music.setLoop(true);

	string path, strCarIndex;
	switch (newScene)
	{
	case MAIN_MENU:
		this->bestDistanceText = Text("Best Distance: " + ConfigManager::loadProperty(this->configPath, "bestStats", ',')[0] + "m", this->textFont, 20U);
		this->bestPointsText = Text("Best Score: " + ConfigManager::loadProperty(this->configPath, "bestStats", ',')[1], this->textFont, 20U);

		if (this->music.openFromFile(musicTracks[0]))
			this->music.play();
		break;
	case FITTING_ROOM:
		path = ConfigManager::loadProperty(this->configPath, "currentCarPath", ',')[0];
		strCarIndex = path.substr(18, path.size() - 18);
		strCarIndex = strCarIndex.substr(0, strCarIndex.size() - 4);

		this->skinIndex = atoi(strCarIndex.c_str());

		this->fittingTex.loadFromFile(path);
		this->fittingSkin = Sprite(this->fittingTex);
		this->fittingSkin.setPosition(this->window->getSize().x / 2 - this->fittingSkin.getLocalBounds().width / 2,
			this->window->getSize().y / 2 - this->fittingSkin.getLocalBounds().height / 2);
		
		break;
	case GAME:
		if (this->music.openFromFile(musicTracks[1]))
			this->music.play();
		this->startTimer.restart();
		this->gameStartedTime = this->gameClock.getElapsedTime().asSeconds();

		this->player->updateSprite();
		this->points = 0;
		this->distance = 0;
		this->speedMultiplier = 1.f;
		this->routineStarted = false;
		break;
	}
}
