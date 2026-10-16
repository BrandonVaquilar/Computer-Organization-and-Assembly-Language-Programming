#include "../include/input.hpp"
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <vector>

extern "C" void asm_spongebob(char*);

void cpp_spongebob(char* text) {
    for (; *text; ++text) {
        const auto value = static_cast<unsigned char>(*text);
        if (std::isalpha(value))
            *text = std::rand() % 2 ? std::toupper(value) : std::tolower(value);
    }
}

int main() try {
    const int seed = read<int>("Please enter a random seed:\n");
    std::ifstream input(read<std::string>("Please enter a filename to spongebob:\n"));
    if (!input) throw std::runtime_error("Cannot open input file");
    std::string text((std::istreambuf_iterator<char>(input)), {});
    if (text.find('\0') != std::string::npos)
        throw std::runtime_error("Input must be a text file without NUL bytes");
    std::vector<char> reference(text.begin(), text.end()), actual(reference);
    reference.push_back('\0');
    actual.push_back('\0');
    std::srand(seed);
    cpp_spongebob(reference.data());
    std::srand(seed);
    asm_spongebob(actual.data());
    if (text.size() <= 1000)
        std::cout << "Before: " << text << "\nARM32: " << actual.data() << '\n';
    if (reference != actual) throw std::runtime_error("ARM32 and C++ results differ");
    std::cout << "PASS: ARM32 matches the C++ reference for seed " << seed << '\n';
} catch (const std::exception& error) {
    std::cerr << error.what() << '\n';
    return 1;
}
