#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    BitmapFileHeader(int w, int h) {
        fheader_size_ = GetBMPStride(w) * h;
    }
    char signature_[2] = {'B', 'M'};
    uint32_t fheader_size_ = {};
    uint32_t fheader_reserved_ = 0;
    uint32_t fheader_offset_ = 54;
    
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    BitmapInfoHeader(int w, int h) :
        iheader_width_(w),
        iheader_heigth_(h)
    {
        iheader_data_bytes_ = GetBMPStride(w) * h;
    }
    uint32_t iheader_size_ = 40;
    uint32_t iheader_width_ = {};
    uint32_t iheader_heigth_ = {};
    uint16_t iheader_planes_ = 1;
    uint16_t iheader_bit_for_pix_ = 24;
    uint32_t iheader_compression_ = 0;
    uint32_t iheader_data_bytes_ = {};
    uint32_t iheader_hor_resolution_ = 11811;
    uint32_t iheader_vert_resolution_ = 11811;
    uint32_t iheader_colours_used_ = 0;
    uint32_t iheader_colours_significant_ = 0x1000000;
}
PACKED_STRUCT_END

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    ofstream ofs(file, ios::binary);
    
    BitmapFileHeader bmpfheader(image.GetWidth(), image.GetHeight());
    BitmapInfoHeader bmpiheader(image.GetWidth(), image.GetHeight());

    ofs.write(reinterpret_cast<const char*>(&bmpfheader), sizeof(bmpfheader));
    ofs.write(reinterpret_cast<const char*>(&bmpiheader), sizeof(bmpiheader));

    std::vector<char> buff(GetBMPStride(image.GetWidth()));

    for (int y = image.GetHeight()-1; y > 0; --y) {
        const Color* line = image.GetLine(y);
        for (int x = 0; x < image.GetWidth(); ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].b);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].r);
        }
        ofs.write(buff.data(), image.GetWidth() * 3);
    }

    return true;
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    ifstream ifs(file, ios::binary);
    int width, height;
    ifs.ignore(18);
    ifs.read(reinterpret_cast<char*>(&width), sizeof(width));
    ifs.read(reinterpret_cast<char*>(&height), sizeof(height));
    ifs.ignore(28);

    Image result{width, height, Color::Black()};
    std::vector<char> buff(GetBMPStride(result.GetWidth()));

    for (int y = result.GetHeight() - 1; y >= 0; --y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), GetBMPStride(result.GetWidth()));

        for (int x = 0; x < result.GetWidth(); ++x) {
            line[x].b = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].r = static_cast<byte>(buff[x * 3 + 2]);
        }
    }

    return result;
}

}  // namespace img_lib