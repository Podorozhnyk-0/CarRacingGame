#include "ConfigManager.h"

vector<string> ConfigManager::loadProperty(string configPath, string propertyName, char delim)
{
	ifstream stream(configPath);

	if (!stream.is_open())
		throw OpenFileFailedException(string("Couldn't open file by path ") + configPath);

	//[propertyName]=[value]
	while (!stream.eof())
	{
		int endIndex;
		string rawStr, property, rawValue;
		
		getline(stream, rawStr);

		if (rawStr.length() <= 0) continue;

		for (endIndex = 1; rawStr[endIndex] != ']'; endIndex++) {}
		property = rawStr.substr(1, endIndex - 1);

		if (property != propertyName) continue;

		int equalIndex = rawStr.find('=');
		endIndex = equalIndex + 2; // '+ 2' because there are two characters after '=' including it (we need to skip them)
		rawValue = rawStr.substr(endIndex, (rawStr.length() - 1) - endIndex); //usual 'length() - 1' and substract endIndex to exclude ']' symbol (endIndex is start- and end- index in one time)

		return splitString(rawValue, delim);
	}
	stream.close();
	return vector<string>();
}

void ConfigManager::saveProperty(string configPath, string propertyName, string value)
{
	ifstream readStream(configPath);
	vector<string> lines;
	bool propertySet = false;

	if (!readStream.is_open())
		throw OpenFileFailedException(string("Couldn't open file by path ") + configPath);

	while (!readStream.eof())
	{
		int endIndex;
		string rawStr, property, rawValue;
		rawStr = property = "";

		getline(readStream, rawStr);

		if (rawStr.length() <= 0)
		{
			if (!propertySet)
			{
				lines.push_back("[" + propertyName + "]=[" + value + "]");
				propertySet = true;
			}
			continue;
		}

		for (endIndex = 1; rawStr[endIndex] != ']'; endIndex++) {}
		property = rawStr.substr(1, endIndex - 1);

		if (property != propertyName && property != "")
		{
			lines.push_back(rawStr);
		}
		else
		{
			lines.push_back("[" + propertyName + "]=[" + value + "]");
			propertySet = true;
		}
	}
	readStream.close();

	ofstream writeStream(configPath, ios::out | ios::trunc);
	if (!writeStream.is_open())
		throw OpenFileFailedException(string("Couldn't open file by path ") + configPath);

	for (size_t i = 0; i < lines.size(); i++)
	{
		writeStream << lines[i] << endl;
	}
	
	writeStream.close();
}

vector<string> ConfigManager::splitString(string rawValue, char delim)
{
	if (delim == 0)
	{
		return vector<string>({ rawValue });
	}

	string tempStr = "";
	vector<string> result;
	for (size_t i = 0; i <= rawValue.length(); i++) //<= needed for one last iteration to push back last tempStr in vector
	{
		if (rawValue[i] == delim || rawValue[i] == '\0')
		{
			if(!tempStr.empty())
				result.push_back(tempStr);
			tempStr.clear();
		}
		else
		{
			tempStr += rawValue[i];
		}
	}

	return result;
}
