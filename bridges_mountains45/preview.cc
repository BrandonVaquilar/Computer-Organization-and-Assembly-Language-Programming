#include "../include/ppm.hpp"
#include <algorithm>
#include <array>
#include <climits>
#include <iostream>

extern "C" void asm_findpath(int*, int, int, int, int);

int main(int argc, char** argv) try {
    if (argc != 4) {
        std::cerr << "Usage: terrain elevation.dat start_row output.ppm\n";
        return 1;
    }
    std::ifstream input(argv[1]);
    int width = 0, height = 0;
    if (!(input >> width >> height)) throw std::runtime_error("Invalid terrain header");
    const auto count = preview::pixel_count(width, height);
    const int row = std::stoi(argv[2]);
    if (row < 0 || row >= height) throw std::runtime_error("Starting row is outside the terrain");
    std::vector<int> elevations(count);
    int low = INT_MAX, high = INT_MIN;
    for (auto& value : elevations) {
        if (!(input >> value)) throw std::runtime_error("Incomplete elevation data");
        low = std::min(low, value);
        high = std::max(high, value);
    }
    if (static_cast<long long>(high) - low > INT_MAX)
        throw std::runtime_error("Elevation range exceeds ARM32 signed arithmetic");
    // Shift below-sea-level datasets up for the nonnegative ARM routine.
    auto marked = elevations;
    for (auto& value : marked) value = static_cast<int>(static_cast<long long>(value) - low);
    asm_findpath(marked.data(), width, height, row, high - low);
    preview::Image image{width, height, std::vector<unsigned char>(count * 3)};
    constexpr std::array<std::array<int, 3>, 5> palette{{
        {{20, 44, 64}}, {{43, 91, 88}}, {{124, 143, 105}}, {{195, 186, 153}}, {{244, 237, 214}}
    }};
    int path_pixels = 0;
    for (std::size_t i = 0; i < count; ++i) {
        const double level = high == low ? 0 : 4.0 * (static_cast<double>(elevations[i]) - low) / (static_cast<double>(high) - low);
        const int band = std::min(3, static_cast<int>(level));
        const double mix = level - band;
        for (int channel = 0; channel < 3; ++channel)
            image.pixels[3 * i + channel] = static_cast<unsigned char>(palette[band][channel] * (1 - mix) + palette[band + 1][channel] * mix);
        if (marked[i] == -1) {
            image.pixels[3 * i] = 72;
            image.pixels[3 * i + 1] = 207;
            image.pixels[3 * i + 2] = 255;
            ++path_pixels;
        }
    }
    if (path_pixels != width) throw std::runtime_error("Expected one path marker per column");
    preview::save_ppm(argv[3], image);
    std::cout << "ARM32 greedy terrain path: " << width << 'x' << height
              << ", start row " << row << ", " << path_pixels << " marked columns\n";
} catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
}
