#pragma once

#include <exception>

class InvalidRequestException : public std::exception {
    public:
    const char * what () const throw () {
        return "Invalid request";
    }
};