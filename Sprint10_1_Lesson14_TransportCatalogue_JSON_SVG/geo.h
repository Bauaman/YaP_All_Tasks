#pragma once

namespace geo {

struct Coordinates {
    Coordinates(double latt, double longt) :
        lat(latt),
        lng(longt)
    {}
    double lat; // Широта
    double lng; // Долгота
    bool operator==(const Coordinates& other) const {
        return lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Coordinates& other) const {
        return !(*this == other);
    }
};

double ComputeDistance(Coordinates from, Coordinates to);

}  // namespace geo