#include "transport_catalogue.h"

void TransportCatalogue::AddStop(const Stop& stop) {
    Stop* curr_stop = &stops_.emplace_back(std::move(stop));
    name_to_stops_.insert({std::string_view(curr_stop->stop_name_), curr_stop});
}

void TransportCatalogue::AddBus(const Bus& bus) {
    Bus* curr_bus = &buses_.emplace_back(std::move(bus));
    name_to_buses_.insert({std::string_view(curr_bus->bus_name_), curr_bus});
    for(Stop* st : curr_bus->route_stops_) {
        st->buses_through_stop_.emplace(curr_bus->bus_name_);
    }
    if (!bus.is_round_) {
        for (int i = curr_bus->route_stops_.size() -2; i>=0; --i) {
            curr_bus->route_stops_.emplace_back(curr_bus->route_stops_[i]);
        }
    } else {
        if (curr_bus->route_stops_.front() != curr_bus->route_stops_.back()) {
            curr_bus->route_stops_.emplace_back(curr_bus->route_stops_[0]);
        }
    }
}

void TransportCatalogue::SetDistancesBetweenStops(const Stop* stop) {
        for (const auto& [dest_name, dist] : stop->dist_to_stops_) {
            Stop* dest_stop = GetStopByName(dest_name);
            const StopsPair pair {stop, dest_stop};
            const StopsPair rev_pair {dest_stop, stop};
            if (distances_for_stops.count(pair) > 0) {
                distances_for_stops.at(pair) = dist;
            } else {
                distances_for_stops.insert({pair, dist});
            } 
            if (!distances_for_stops.count(rev_pair)) {
                distances_for_stops.insert({rev_pair, dist});
            }
        }
}

Stop* TransportCatalogue::GetStopByName(std::string_view stop_name) const {
    return name_to_stops_.count(stop_name) ? name_to_stops_.at(stop_name) : (std::nullptr_t)nullptr;
}

Bus* TransportCatalogue::GetBusByName(std::string_view bus_name) const {
    return name_to_buses_.count(bus_name) ? name_to_buses_.at(bus_name) : (std::nullptr_t)nullptr;
}

int TransportCatalogue::GetDistanceBetweenStops(Stop* from, Stop* to) const {
    StopsPair pair{from,to};
    return distances_for_stops.at(pair);
}

const std::vector<std::string> TransportCatalogue::GetRoutesForStop (const Stop* st_p) const {
    std::vector<std::string> res (st_p->buses_through_stop_.begin(), st_p->buses_through_stop_.end());
    std::sort(res.begin(), res.end());
    return res;
}

std::map<std::string_view, const Bus*> TransportCatalogue::GetSortedBuses() const {
    std::map<std::string_view, const Bus*> res;
    for (auto& bus : buses_) {
        res.emplace(bus.bus_name_, &bus);
    }
    return res;
}

std::map<std::string_view, const Stop*> TransportCatalogue::GetSortedStops() const {
    std::map<std::string_view, const Stop*> res;
    for (auto& bus: buses_) {
        if (bus.route_stops_.size()==0) {
            continue;
        }
        for (auto& stop : bus.route_stops_) {
            res.emplace(stop->stop_name_, stop);
        }
    }
    return res;
}

const std::unordered_map<std::string_view, Stop*> TransportCatalogue::GetAllStops() const {
    return name_to_stops_;
}