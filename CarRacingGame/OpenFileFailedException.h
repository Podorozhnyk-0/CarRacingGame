#pragma once
#include <iostream>

using namespace std;

class OpenFileFailedException : public exception
{
public:
	OpenFileFailedException(const string& message);

	const char* what() const noexcept override;

private:
	string message;
};

