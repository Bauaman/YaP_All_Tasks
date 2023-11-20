#include "domain.h"
size_t Bus::GetUniqueStopsCount() const {
    return unique_stops_.size();
}

size_t Bus::GetStopsCount() const {
    return route_stops_.size();
}