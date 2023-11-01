#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "transport_catalogue.h"

namespace transport {

        void TransportCatalogue::AddBus (const Bus& bus) {
            buses_.push_back(bus);
            name_to_bus_[bus.route_name_] = &buses_.back();
            for (const auto& st : bus.route_stops_) {
                //const Stop* st_p = name_to_stop_.at(st);
                routes_for_stop_[st].emplace(&bus);
            }
        }
        /*
        void TransportCatalogue::SetRoutesForStop(const Bus& bus) {
            for (const auto& st : bus.route_stops_) {
                //const Stop* st_p = name_to_stop_.at(st);
                routes_for_stop_[st].emplace(&bus);
            }
        }
        */
        void TransportCatalogue::AddStop (const Stop& stop) {
            stops_.insert(stops_.end(), stop);
            name_to_stop_[stop.stop_name_] = &stops_.back();
            routes_for_stop_[&stops_.back()] = {};
        }

        const Bus* TransportCatalogue::GetBusByName (const std::string& bus_name) const {
            return name_to_bus_.count(bus_name) ? name_to_bus_.at(bus_name) : (std::nullptr_t)nullptr;
        }

        const Stop* TransportCatalogue::GetStopByName (const std::string& stop_name) const {
            //std::cout << stop_name << std::endl;
            return name_to_stop_.count(stop_name) ? name_to_stop_.at(stop_name) : (std::nullptr_t)nullptr;
        }

        void TransportCatalogue::SetDistance(const Stop* from, const Stop* to, int dist) {
            StopsPair stops {from, to};
            StopsPair stops_rev {to,from};
            if (distances_between_stops_.count(stops) > 0) {
                distances_between_stops_.at(stops) = dist;
            } else {
                distances_between_stops_.insert({stops, dist});
            }
            if (!distances_between_stops_.count(stops_rev)) {
                distances_between_stops_.insert({stops_rev, dist});
            }
        }

        int TransportCatalogue::GetDistance(const Stop* from, const Stop* to) const {
            StopsPair stops{from,to};
            return distances_between_stops_.at(stops);
        }

        const std::vector<const Bus*> TransportCatalogue::GetRoutesForStop (const Stop* st_p) const {
                std::vector<const Bus*> res (routes_for_stop_.at(st_p).begin(), routes_for_stop_.at(st_p).end());
                std::sort(res.begin(), res.end(), compareBuses());
                //std::unordered_set<const Bus*> result(res.begin(), res.end());
                return res;
        }

}//namespace transport