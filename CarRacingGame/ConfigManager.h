#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "OpenFileFailedException.h"

using namespace std;

class ConfigManager
{
public:

	static vector<string> loadProperty(string configPath, string propertyName, char delim = 0);

	static void saveProperty(string configPath, string propertyName, string value);
private:
	static vector<string> splitString(string rawValue, char delim = ' ');
};
