#pragma once

#include <exception>

class FullBufferException : public std::exception {
    public:
    const char * what () const throw () {
        return "Buffer is full";
    }
};