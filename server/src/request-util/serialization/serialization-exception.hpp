#pragma once
#include <string>

class SerializationException : public std::exception
{
private:
    std::string message;
public:
    SerializationException(std::string message) {this->message =  "Serialization failed: " + message;}
    SerializationException() {this->message = "Serialization failed";}
    const char *what() const throw()
    {
        return this->message.c_str();
    }
};