#pragma once
#include <iostream>
#include <stdexcept>
#include <string>

// Replacement for the course server's /public/read.h dependency.
template <typename T = int>
T read(const std::string& prompt) {
    std::cout << prompt;
    T value{};
    if (!(std::cin >> value)) throw std::runtime_error("Invalid or missing input");
    return value;
}
