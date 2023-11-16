#include "transport_catalogue.h"

/*
 * Здесь можно разместить код транспортного справочника
 */
namespace Transport {

    void TransportCatalogue::AddStop(const Stop* stop) {
        //std::cout << "adding stop " << stop->stop_name_ << std::endl;
        stops_.emplace_back(std::move(*stop));
        std::string_view stop_name = stops_.back().stop_name_;
        name_to_stops_[stop_name] = &stops_.back();
    }

    void TransportCatalogue::AddBus(const Bus& bus) {
        //std::cout << "adding bus " << bus.bus_name_ << std::endl;
        buses_.emplace_back(bus);
        std::string_view bus_name_ = buses_.back().bus_name_;
        name_to_bus_[bus_name_] = &buses_.back();
        for (const Stop* stop : bus.bus_stops_) {
            const_cast<Stop*>(stop)->buses_for_stop_.emplace(bus_name_);
            //const_cast<Stop*>(stop)->buses_for_stop_.emplace(&buses_.back());
        }
    }

    const Stop* TransportCatalogue::GetStopByName(std::string_view stop_name) const {
        return name_to_stops_.count(stop_name) ? name_to_stops_.at(stop_name) : (std::nullptr_t)nullptr;
    }

    const Bus* TransportCatalogue::GetBusByName(std::string_view bus_name) const {
        return name_to_bus_.count(bus_name) ? name_to_bus_.at(bus_name) : (std::nullptr_t)nullptr;
    }

    size_t TransportCatalogue::GetBusQty() {
        return buses_.size();
    }

    size_t TransportCatalogue::GetStopQty() {
        return stops_.size();
    }

    void TransportCatalogue::SetDistance (const Stop* from, const Stop* to, int dist) {
        const StopsPair stops {from, to};
        const StopsPair stops_rev {to, from};
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

} //namespace Transport