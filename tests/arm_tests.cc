#include <algorithm>
#include <array>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

extern "C" {
int foo(int); int bar(int, int); int baz(int, int, int); int quz(int, int, int, int);
void sdarken(unsigned char*, unsigned char*, int, int);
void ssepia(unsigned char*, unsigned char*, int, int);
void sfilter2(unsigned char*, unsigned char*, int, int);
void asm_findpath(int*, int, int, int, int);
}

void check(bool value, const char* message) {
    if (!value) throw std::runtime_error(message);
}

int main() try {
    std::mt19937 random(2026);
    for (int i = 0; i < 1000; ++i) {
        const int x = static_cast<int>(random() % 101) - 50;
        const int y = static_cast<int>(random() % 101) - 50;
        const int z = static_cast<int>(random() % 101) - 50;
        const int w = static_cast<int>(random() % 101) - 50;
        check(foo(x) == 4*x, "foo mismatch");
        check(bar(x,y) == x*y-y*y, "bar mismatch");
        check(baz(x,y,z) == 5*x+5*x*y+5*x*y*z, "baz mismatch");
        check(quz(x,y,z,w) == 2*(x-y)*(z+w), "quz mismatch");
    }
    std::cout << "PASS: 4,000 signed arithmetic comparisons\n";

    using Filter = void (*)(unsigned char*, unsigned char*, int, int);
    const std::array<Filter, 3> filters{{sdarken, ssepia, sfilter2}};
    const int coefficients[2][3][3] = {
        {{101,197,48},{90,176,43},{70,137,34}},
        {{500,197,48},{120,176,43},{200,137,34}}
    };
    for (int width : {1,2,7,31}) for (int height : {1,3,8}) {
        const int count = width*height;
        std::vector<unsigned char> input(count*3);
        for (auto& value : input) value = random() % 256;
        if (count >= 2) for (int c = 0; c < 3; ++c) {
            input[c*count] = 0; input[c*count+1] = 255;
        }
        for (int filter = 0; filter < 3; ++filter) {
            std::vector<unsigned char> output(count*3+32, 0xA5);
            filters[filter](input.data(), output.data()+16, width, height);
            for (int channel = 0; channel < 3; ++channel) for (int i = 0; i < count; ++i) {
                int expected = input[channel*count+i] / 2;
                if (filter) {
                    expected = 0;
                    for (int source = 0; source < 3; ++source)
                        expected += coefficients[filter-1][channel][source]*input[source*count+i] / 256;
                    expected = std::min(255, expected);
                }
                check(output[16+channel*count+i] == expected, "filter pixel mismatch");
            }
            check(std::all_of(output.begin(), output.begin()+16, [](auto x){return x==0xA5;}), "filter wrote before output");
            check(std::all_of(output.end()-16, output.end(), [](auto x){return x==0xA5;}), "filter wrote beyond output");
        }
    }
    std::cout << "PASS: 36 filter cases, exact channels, saturation and buffer guards\n";

    int cases = 0;
    for (int width : {1,2,7,23}) for (int height : {1,2,5})
        for (int start = 0; start < height; ++start) for (int trial = 0; trial < 12; ++trial) {
            std::vector<int> source(width*height);
            for (auto& value : source) value = trial == 0 ? 10 : random()%10000;
            if (trial == 1 && source.size() > 1) { source[0] = INT_MAX; source[1] = 0; }
            auto expected = source;
            int row = start;
            for (int column = 0; column < width; ++column) {
                expected[row*width+column] = -1;
                if (column+1 == width) break;
                const int current = source[row*width+column];
                int next = row;
                long long best = std::llabs(static_cast<long long>(source[row*width+column+1])-current);
                for (int candidate : {row-1,row+1}) if (candidate >= 0 && candidate < height) {
                    const auto change = std::llabs(static_cast<long long>(source[candidate*width+column+1])-current);
                    if (change < best) { best = change; next = candidate; }
                }
                row = next;
            }
            std::vector<int> guarded(source.size()+2, 123456);
            std::copy(source.begin(), source.end(), guarded.begin()+1);
            asm_findpath(guarded.data()+1, width, height, start, 10000);
            check(guarded.front()==123456 && guarded.back()==123456, "terrain buffer guard changed");
            check(std::equal(expected.begin(), expected.end(), guarded.begin()+1), "terrain route mismatch");
            ++cases;
        }
    std::vector<int> invalid{1,2,3,4};
    for (int start : {-1,2,100}) asm_findpath(invalid.data(),2,2,start,4);
    asm_findpath(invalid.data(),0,2,0,4);
    asm_findpath(invalid.data(),2,0,0,4);
    asm_findpath(nullptr,2,2,0,4);
    check(invalid == std::vector<int>({1,2,3,4}), "invalid terrain input changed data");
    std::cout << "PASS: " << cases << " terrain routes, ties, borders, final column and invalid arguments\n";
} catch (const std::exception& error) {
    std::cerr << "FAIL: " << error.what() << '\n';
    return 1;
}
