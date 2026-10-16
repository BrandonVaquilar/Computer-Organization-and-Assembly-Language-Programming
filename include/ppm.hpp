#pragma once
#include <cctype>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

namespace preview {
struct Image {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> pixels; // Interleaved RGB.
};

inline std::size_t pixel_count(int width, int height) {
    if (width <= 0 || height <= 0 || width > 16000000 / height)
        throw std::runtime_error("Dimensions must be positive and at most 16 million pixels");
    return static_cast<std::size_t>(width) * height;
}

inline std::string token(std::istream& in) {
    while (in >> std::ws && in.peek() == '#')
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string result;
    if (!(in >> result)) throw std::runtime_error("Incomplete PPM header");
    return result;
}

inline Image load_ppm(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Cannot open input: " + path);
    if (token(in) != "P6") throw std::runtime_error("Use a binary RGB PPM (P6) image");
    Image image;
    image.width = std::stoi(token(in));
    image.height = std::stoi(token(in));
    if (token(in) != "255") throw std::runtime_error("PPM maximum must be 255");
    int separator = in.get();
    if (separator == '\r' && in.peek() == '\n') in.get();
    if (separator == EOF || !std::isspace(static_cast<unsigned char>(separator)))
        throw std::runtime_error("Missing PPM header separator");
    image.pixels.resize(pixel_count(image.width, image.height) * 3);
    if (!in.read(reinterpret_cast<char*>(image.pixels.data()), image.pixels.size()))
        throw std::runtime_error("Incomplete PPM pixels");
    return image;
}

inline void save_ppm(const std::string& path, const Image& image) {
    if (image.pixels.size() != pixel_count(image.width, image.height) * 3)
        throw std::runtime_error("Incorrect RGB buffer size");
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Cannot open output: " + path);
    out << "P6\n" << image.width << ' ' << image.height << "\n255\n";
    out.write(reinterpret_cast<const char*>(image.pixels.data()), image.pixels.size());
    if (!out) throw std::runtime_error("Failed to write output: " + path);
}
} // namespace preview
