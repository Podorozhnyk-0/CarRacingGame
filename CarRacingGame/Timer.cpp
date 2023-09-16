#include "Timer.h"

void Timer::restart()
{
	this->time.restart();

	this->millisecondsLeft = this->endTime;

	this->running = true;
}

void Timer::update()
{
	if (this->endTime - this->time.getElapsedTime().asMilliseconds() <= 0)
	{
		this->running = false;
		return;
	}
	if (!this->running) this->running = true;

	this->millisecondsLeft = this->endTime - this->time.getElapsedTime().asMilliseconds();
}
