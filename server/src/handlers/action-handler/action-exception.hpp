#pragma once
#include <exception>

class ActionUnknownException : public std::exception {
    public:
    ActionUnknownException() {}
    const char *what() const throw() {
        return "Action exception: Unknown action";
    }
};