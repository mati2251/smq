#pragma once
#include <exception>
#include <string>

class AcceptException : public std::exception
{
public:
    AcceptException() {}
    const char *what() const throw()
    {
        return "AcceptException";
    }
};
