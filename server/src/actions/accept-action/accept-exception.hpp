#pragma once
#include <exception>

class AcceptException final : public std::exception {
public:
    AcceptException() = default;

    const char *what() const noexcept override {
        return "AcceptException";
    }
};
