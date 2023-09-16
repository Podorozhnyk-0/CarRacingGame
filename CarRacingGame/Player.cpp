#include "Player.h"

Player::Player(string configPath, RenderWindow* window, vector<Vector2f> points)
{
	this->configPath = configPath;
	this->window = window;

	this->currentPos = 0;
	this->isAlive = true;

	this->availablePoints = points;

	string texturePath = ConfigManager::loadProperty(this->configPath, "currentCarPath")[0];

	this->carTex.loadFromFile(texturePath);
	this->carSprite = Sprite(this->carTex);

	this->carSprite.setPosition(points[this->currentPos]);
}

void Player::move(Keyboard::Key direction)
{
	if (direction == Keyboard::Left && this->currentPos > 0)
	{
		this->currentPos--;
	}
	if (direction == Keyboard::Right && this->currentPos < this->availablePoints.size() - 1)
	{
		this->currentPos++;
	}
	this->carSprite.setPosition(this->availablePoints[this->currentPos]);
}

bool Player::checkCollision(Sprite& other)
{
	return this->carSprite.getGlobalBounds().intersects(other.getGlobalBounds());
}

void Player::updateSprite()
{
	string texturePath = ConfigManager::loadProperty(this->configPath, "currentCarPath")[0];
	this->carTex.loadFromFile(texturePath);
}

void Player::draw(RenderTarget& target, RenderStates states) const
{
	this->window->draw(this->carSprite);
}
