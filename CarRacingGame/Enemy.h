#pragma once
#include "SFML/Graphics.hpp"
#include "ConfigManager.h"

using namespace sf;

class Enemy : public Drawable
{
public:
	void draw(RenderTarget& target, RenderStates states) const;

	void update();

	inline Vector2f getPosition() { return this->carSprite.getPosition(); }

	inline Sprite getSprite() { return this->carSprite; }

	static Enemy* create(string configPath, RenderWindow* window, float* speedMultiplier, vector<Vector2f> points);

	~Enemy();

private:
	//private methods
	Enemy(string configPath, RenderWindow* window, float* speedMultiplier, vector<Vector2f> points);

private:
	RenderWindow* window;

	vector<Vector2f> availablePoints;

	string configPath; 

	Vector2f currentPos;

	float* speed;

	Texture carTex;
	Sprite carSprite;
};

