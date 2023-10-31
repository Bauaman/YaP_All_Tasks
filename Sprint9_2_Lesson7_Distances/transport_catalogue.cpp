#include <stdexcept>
#include "transport_catalogue.h"

namespace transport {

        void TransportCatalogue::AddBus (const Bus& bus) {
            buses_.push_back(bus);
            name_to_bus_[bus.route_name_] = &buses_.back();

        }

        void TransportCatalogue::AddStop (const Stop& stop) {
            stops_.insert(stops_.end(), stop);
            name_to_stop_[stop.stop_name_] = &stops_.back();
            routes_for_stop_[&stops_.back()] = {};
        }

        const Bus* TransportCatalogue::GetBusByName (const std::string& bus_name) const {
            return name_to_bus_.count(bus_name) ? name_to_bus_.at(bus_name) : (std::nullptr_t)nullptr;
        }

        const Stop* TransportCatalogue::GetStopByName (const std::string& stop_name) const {
            return name_to_stop_.count(stop_name) ? name_to_stop_.at(stop_name) : (std::nullptr_t)nullptr;
        }

        const Route* TransportCatalogue::GetRouteByName (const std::string& route_name) const {
            return name_to_route_.count(route_name) ? name_to_route_.at(route_name) : (std::nullptr_t)nullptr;
        }

        void TransportCatalogue::FillStopToRoute(const Bus& bus) {
            for (const auto& st : bus.route_stops_) {

                Stop* st_p = name_to_stop_.at(st);
                routes_for_stop_[st_p].emplace(bus.route_name_);
            }
        }

        void TransportCatalogue::FillDistBwStops(Key stops, int dist) {
            if (distances_between_stops_.count(stops) > 0) {
                distances_between_stops_.at(stops) = dist;
            } else {
                distances_between_stops_.insert({stops, dist});
            }
        }

        bool TransportCatalogue::CheckIfContainsPair(Key& stops) {
            return distances_between_stops_.count(stops) > 0;
        }

        int TransportCatalogue::GetDistance(Key& stops) {
            return distances_between_stops_.at(stops);
        }

        const std::set<std::string> TransportCatalogue::GetRoutesForStop (const Stop* st_p) const {
                return routes_for_stop_.at(st_p);
        }

        void TransportCatalogue::AddRoute(const Bus& bus) {
            Route* route = new Route();
            std::unordered_set<std::string_view> unique_stops;
            route->stops_count_ = bus.route_stops_.size();
            for (size_t i=0; i<bus.route_stops_.size()-1; ++i) {
                Stop* from = name_to_stop_.at(bus.route_stops_[i]);
                Stop* to = name_to_stop_.at(bus.route_stops_[i+1]);
                geo::Coordinates coord_from = from->coord_;
                geo::Coordinates coord_to = to->coord_;
                route->route_length_ += geo::ComputeDistance(coord_from, coord_to);
                Key route_key = {from,to};
                route->route_distance_ += GetDistance(route_key);
                unique_stops.emplace(bus.route_stops_[i]);
                route->curvature_ = route->route_distance_*1.0 / route->route_length_;
            }
            
            route->unique_stops_count_ = unique_stops.size();
            name_to_route_[bus.route_name_] = route;
        }

}//namespace transport