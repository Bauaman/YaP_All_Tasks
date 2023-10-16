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
                if (name_to_stop_.count(st)) {
                    name_to_stop_.at(st)->buses_for_stop_.emplace(bus.route_name_);
                }
                routes_for_stop_[st].emplace(bus.route_name_);
            }
        }

        const std::set<std::string> TransportCatalogue::GetRoutesForStop (const std::string& stop_name) const {
            if (routes_for_stop_.count(stop_name)) {
                return routes_for_stop_.at(stop_name);
            }
            else {
                throw std::invalid_argument("stop doesn't exist");
            }
        }

        void TransportCatalogue::AddRoute(const Bus& bus) {
            Route* route = new Route();
            std::unordered_set<std::string_view> unique_stops;
            route->stops_count_ = bus.route_stops_.size();
            for (size_t i=0; i<bus.route_stops_.size()-1; ++i) {
                geo::Coordinates from = name_to_stop_.at(bus.route_stops_[i])->coord_;
                geo::Coordinates to = name_to_stop_.at(bus.route_stops_[i+1])->coord_;
                route->route_length_ += geo::ComputeDistance(from, to);
                unique_stops.emplace(bus.route_stops_[i]);
            }
            
            route->unique_stops_count_ = unique_stops.size();
            name_to_route_[bus.route_name_] = route;
        }

}//namespace transport