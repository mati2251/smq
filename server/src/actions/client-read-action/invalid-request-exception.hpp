#pragma once

#include <exception>

class InvalidRequestException final : public std::exception {
    public:
     const char * what () const noexcept override {
        return "Invalid request";
    }
};
