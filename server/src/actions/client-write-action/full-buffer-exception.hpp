#pragma once

#include <exception>

class FullBufferException final : public std::exception {
    public:
    const char * what () const noexcept override {
        return "Buffer is full";
    }
};
