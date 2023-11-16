#pragma once

#include <algorithm>
#include <deque>
#include <string>
#include <unordered_map>
#include "domain.h"

class TransportCatalogue {
public:
    TransportCatalogue() = default;

    void AddStop(Stop&& stop);
    void AddBus(Bus&& bus);
    Stop* GetStopByName(const std::string_view stop_name) const;
    Bus* GetBusByName(const std::string_view bus_name) const;

    void SetDistancesBetweenStops();
    int GetDistanceBetweenStops(Stop* from, Stop* to) const;
    const std::vector<std::string> GetRoutesForStop (const Stop* st_p) const;

private:
/*
    struct StopsPairHasher {
        size_t operator()(const StopsPair& pair) const {
            return pair.first->Hash() + prime_num * pair.second->Hash();
        }        
        static const size_t prime_num{47};
    };
*/
	struct StopsPairHasher {
		size_t operator()(const StopsPair& stops) const {
			std::hash<std::string> hasher;
			return (hasher(stops.stop_from->stop_name_)*100 + hasher(stops.stop_to->stop_name_)*10);
		}
	};
    
    std::deque<Stop> stops_;
    std::unordered_map<std::string_view, Stop*> name_to_stops_;

    std::deque<Bus> buses_;
    std::unordered_map<std::string_view, Bus*> name_to_buses_;

    std::unordered_map<StopsPair, int, StopsPairHasher> distances_for_stops;

};
