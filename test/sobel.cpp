#include "ppm_image.h"

#include <algorithm>
#include <array>
#include <cmath>
#include <iostream>
#include <string_view>

using namespace std;

int Sum(img_lib::Color c) {
    return to_integer<int>(c.r) + to_integer<int>(c.g) + to_integer<int>(c.b);
}

// реализуйте оператор Собеля
img_lib::Image Sobel(const img_lib::Image& image) {
    //fill firs and last line with black
    img_lib::Image res(image.GetWidth(), image.GetHeight(), img_lib::Color::Black());
    for (int y = 1; y < image.GetHeight() - 2; ++y) {
        for (int x = 1; x < image.GetWidth() -2; ++x) {
            int gx = 0;
            int gy = 0;
            for (int dy = -1; dy < 2; ++dy) {
                for (int dx = -1; dx < 2; ++dx) {
                    if (dy != 0) {
                        int kx = 2 - std::abs(dx);
                        gx += dy * kx * Sum(image.GetPixel(x+dx,y+dy));
                    }
                    if (dx != 0) {
                        int ky = 2 - std::abs(dy);
                        gy += dx * ky * Sum(image.GetPixel(x+dx,y+dy));
                    }
                }
            }
            std::byte sob = static_cast<byte>(std::clamp<double>(std::sqrt(pow(gx,2) + pow(gy,2)),0,255));
            res.GetPixel(x,y).r = res.GetPixel(x,y).g = res.GetPixel(x,y).b = sob;
        }
    }
    return res;
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
        return 1;
    }

    auto image = img_lib::LoadPPM(argv[1]);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    image = Sobel(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}