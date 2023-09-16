#include "OpenFileFailedException.h"

OpenFileFailedException::OpenFileFailedException(const string& message)
{
    this->message = message;
}

const char* OpenFileFailedException::what() const noexcept
{
    return message.c_str();
}
