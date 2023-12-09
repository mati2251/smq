#pragma once
#include <exception>

class ConnectionCloseException : public std::exception {
    public:
    ConnectionCloseException() {}
    const char *what() const throw() {
        return "Connection closed";
    }
};