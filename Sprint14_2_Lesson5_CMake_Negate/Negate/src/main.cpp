#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

byte Negate(byte c) {
    return byte(255 - to_integer<int>(c));
}

img_lib::Color Negate(img_lib::Color& c) {
    return {Negate(c.r), Negate(c.g), Negate(c.b), std::byte(255)};
}

void NegateInplace(img_lib::Image& image) {
    for (int h = 0; h<image.GetHeight(); ++h) {
        for (int w = 0; w<image.GetWidth(); ++w) {
            img_lib::Color& cur_pixel = image.GetPixel(w,h);
            cur_pixel = Negate(cur_pixel);
        }
    }
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

    NegateInplace(image);

    if (!img_lib::SavePPM(argv[2], image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}
