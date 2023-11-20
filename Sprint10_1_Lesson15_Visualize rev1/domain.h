#pragma once

#include <set>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>

#include "geo.h"

/*
enum class RouteType {
    ROUND,
    NOT_ROUND
};
*/
struct Stop {
    Stop() = default;
    Stop (const std::string stop_name, const geo::Coordinates& coord) :
        stop_name_(stop_name),
        coord_(coord)
    {}
    std::string stop_name_;
    geo::Coordinates coord_;
    std::unordered_set<std::string> buses_through_stop_;
    std::unordered_map<std::string, int> dist_to_stops_;

    bool operator==(const Stop other) {
        return stop_name_ == other.stop_name_;
    }

    bool operator!=(const Stop other) {
        return stop_name_!=other.stop_name_;
    }
};

struct Bus {
    Bus() = default;
    Bus (const std::string bus_name, bool is_round) :
        bus_name_(bus_name),
        is_round_(is_round)
    {}
    size_t GetStopsCount() const;
    size_t GetUniqueStopsCount() const;
    std::string bus_name_;
    Stop* pivot_stop_;
    std::vector<Stop*> route_stops_;
    std::unordered_set<Stop*> unique_stops_;
    bool is_round_ = 0;
};




struct BusInfo {
    int request_id_;
    std::string bus_no_;
    int stops_count_ = 0;
    int unique_stops_count_ = 0;
    int route_length_ = 0;
    double route_geo_distance = 0.;
    double curvature = 0.;
};

struct StopsPair {
	const Stop* stop_from;
	const Stop* stop_to;

	bool operator==(const StopsPair& other) const {
		return stop_from == other.stop_from && stop_to == other.stop_to;
	}
};

