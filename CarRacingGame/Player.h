#pragma once
#include "SFML/Graphics.hpp"
#include "ConfigManager.h"

using namespace std;
using namespace sf;

class Player : public Drawable
{
public:
	Player(string configPath, RenderWindow* window, vector<Vector2f> points);

	void move(Keyboard::Key direction);

	bool checkCollision(Sprite& other);

	void updateSprite();

	inline int getCurrentPos() { return currentPos; }

	void draw(RenderTarget& target, RenderStates states) const override;
private:
	string configPath;

	vector<Vector2f> availablePoints;

	Texture carTex;
	Sprite carSprite;

	RenderWindow* window;

	int currentPos;
	bool isAlive;
};

