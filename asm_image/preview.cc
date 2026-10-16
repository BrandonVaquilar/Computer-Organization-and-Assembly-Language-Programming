#include "../include/ppm.hpp"
#include <iostream>

extern "C" {
void sdarken(unsigned char*, unsigned char*, int, int);
void ssepia(unsigned char*, unsigned char*, int, int);
void sfilter2(unsigned char*, unsigned char*, int, int);
}

int main(int argc, char** argv) try {
    if (argc != 4) {
        std::cerr << "Usage: image_filters darken|sepia|warm input.ppm output.ppm\n";
        return 1;
    }
    const std::string filter = argv[1];
    auto image = preview::load_ppm(argv[2]);
    const auto count = preview::pixel_count(image.width, image.height);
    std::vector<unsigned char> planar(count * 3), result(count * 3);
    for (std::size_t i = 0; i < count; ++i)
        for (std::size_t channel = 0; channel < 3; ++channel)
            planar[channel * count + i] = image.pixels[3 * i + channel];
    if (filter == "darken") sdarken(planar.data(), result.data(), image.width, image.height);
    else if (filter == "sepia") ssepia(planar.data(), result.data(), image.width, image.height);
    else if (filter == "warm") sfilter2(planar.data(), result.data(), image.width, image.height);
    else throw std::runtime_error("Unknown filter: " + filter);
    for (std::size_t i = 0; i < count; ++i)
        for (std::size_t channel = 0; channel < 3; ++channel)
            image.pixels[3 * i + channel] = result[channel * count + i];
    preview::save_ppm(argv[3], image);
    std::cout << "ARM32 " << filter << ": " << image.width << 'x' << image.height << " -> " << argv[3] << '\n';
} catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
}
