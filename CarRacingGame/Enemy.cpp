#include "Enemy.h"

Enemy::Enemy(string configPath, RenderWindow* window, float* speedMultiplier, vector<Vector2f> points)
{
	this->window = window;
	this->configPath = configPath;
	this->speed = speedMultiplier;
	this->availablePoints = points;

	auto properties = ConfigManager::loadProperty(this->configPath, "enemyCarsPath", ',');
	int random = rand() % properties.size();
	string texturePath = properties[random];

	this->carTex.loadFromFile(texturePath);
	this->carSprite = Sprite(carTex);
	this->carSprite.scale(-1, -1);
	this->carSprite.setOrigin(this->carSprite.getGlobalBounds().width / 2, this->carSprite.getGlobalBounds().height / 2);

	random = rand() % this->availablePoints.size();
	this->currentPos = this->availablePoints[random];
	//this->currentPos.y = this->currentPos.y * 2 / (rand() % 4);
	this->carSprite.setPosition(this->currentPos);
}

void Enemy::draw(RenderTarget& target, RenderStates states) const
{
	this->window->draw(this->carSprite);
}

void Enemy::update()
{
	this->carSprite.move(0, *this->speed);
}

Enemy* Enemy::create(string configPath, RenderWindow* window, float* speedMultiplier, vector<Vector2f> points)
{
	return new Enemy(configPath, window, speedMultiplier, points);
}

Enemy::~Enemy()
{
	
}
