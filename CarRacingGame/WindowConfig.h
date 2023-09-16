#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"

using namespace sf;
using namespace std;

struct WindowConfig
{
	VideoMode mode;
	string title;
	Uint32 style;
	Uint32 fpsLimit;
};