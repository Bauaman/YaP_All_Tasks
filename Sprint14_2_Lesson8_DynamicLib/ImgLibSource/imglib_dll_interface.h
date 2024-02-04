#pragma once

#include <cstddef>

#include "imglib_dll_defines.h"

struct ImgLibSize {
    int width;
    int height;
};

struct ImgLibColor {
    static ImgLibColor Black() {
        return {std::byte{0}, std::byte{0}, std::byte{0}, std::byte{255}};
    }
    std::byte r, g, b, a;
};

class ImgLibImage;

IMGLIB_EXPORT ImgLibImage* ImgLibImgCreate(int width, int height, ImgLibColor fill);
IMGLIB_EXPORT void ImgLibImgFree(ImgLibImage* img);

IMGLIB_EXPORT int ImgLibImgGetWidth(const ImgLibImage* img);
IMGLIB_EXPORT int ImgLibImgGetHeight(const ImgLibImage* img);
IMGLIB_EXPORT int ImgLibImgGetStep(const ImgLibImage* img);
IMGLIB_EXPORT bool ImgLibImgIsInited(const ImgLibImage* img);
IMGLIB_EXPORT ImgLibColor ImgLibImgGetPixelConst(const ImgLibImage* img, int x, int y);
IMGLIB_EXPORT ImgLibColor& ImgLibImgGetPixel(ImgLibImage* img, int x, int y);
IMGLIB_EXPORT const ImgLibColor* ImgLibImgGetLineConst(const ImgLibImage* img, int line);
IMGLIB_EXPORT ImgLibColor* ImgLibImgGetLine(ImgLibImage* img, int line);

IMGLIB_EXPORT ImgLibImage* ImgLibLoadPPM(const char* path);

inline void ImgLibUnloadPPM(ImgLibImage* image) {
    ImgLibImgFree(image);
}

IMGLIB_EXPORT bool ImgLibSavePPM(const char* path, const ImgLibImage* image); 