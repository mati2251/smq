#pragma once
#include <exception>

class ConnectionCloseException final : public std::exception {
    public:
    ConnectionCloseException() = default;
     const char *what() const noexcept override {
        return "Connection closed";
    }
};
