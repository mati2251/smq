#pragma once
#include <string>

class DeserializationException final : public std::exception
{
    std::string message;
public:
    explicit DeserializationException(const std::string &message) {this->message =  "Deserialization failed: " + message;}
    DeserializationException() {this->message = "Deserialization failed";}
    const char *what() const throw()
    {
        return this->message.c_str();
    }
};
