#pragma once
#include <exception>

class ActionException : public std::exception {
    public:
    ActionException() {}
    const char *what() const throw() {
        return "Action exception: Unknown action";
    }
};