#pragma once
#include <exception>

class ActionUnknownException final : public std::exception {
    public:
    ActionUnknownException() = default;
    const char *what() const noexcept override {
        return "Action exception: Unknown action";
    }
};
