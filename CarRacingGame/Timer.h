#pragma once
#include "SFML/Graphics.hpp"

using namespace sf;

class Timer
{
public:
	inline void setEndTime(float newTime) { this->endTime = newTime; }

	inline float getLeftSeconds() { return this->millisecondsLeft / 1000.f; };

	inline float getLeftMilliseconds() { return this->millisecondsLeft; };

	inline float getLeftMinutes() { return this->getLeftSeconds() / 60.f; };

	inline bool isRunning() { return this->running; }

	void restart();

	void update();
private:
	Clock time;

	float endTime;

	float millisecondsLeft;

	bool running;
};

