#include "buildings.h"

using namespace std;

/* 5 */ buildings::Skyscraper::Floor /* 6 */ buildings::Skyscraper::BuildFloor(/* 7 */ Materials materials) {
    /* 8 */ Materials materials_copy = move(materials);
    return Floor{move(materials_copy), static_cast<int>(floors_.size())};
}

namespace buildings {
    void /* 9 */ Skyscraper::BuildRoof(/* 10 */Materials materials) {
        /* 11 */ Materials materials_copy = move(materials);
        roof_ = move(materials_copy);
    }
}  // namespace buildings 