#pragma once
#include <string>

class DeserializationException : public std::exception
{
private:
    std::string message;
public:
    DeserializationException(std::string message) {this->message =  "Deserialization failed: " + message;}
    DeserializationException() {this->message = "Deserialization failed";}
    const char *what() const throw()
    {
        return this->message.c_str();
    }
};