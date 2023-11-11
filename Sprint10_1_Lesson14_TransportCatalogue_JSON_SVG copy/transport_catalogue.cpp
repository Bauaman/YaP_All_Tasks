#include "transport_catalogue.h"

/*
 * Здесь можно разместить код транспортного справочника
 */
namespace Transport {

    void TransportCatalogue::AddStop(const Domain::Stop* stop) {
        std::cout << "adding stop " << stop->stop_name_ << std::endl;
        stops_.emplace_back(std::move(*stop));
        std::string_view stop_name = stops_.back().stop_name_;
        name_to_stops_[stop_name] = &stops_.back();
    }

    void TransportCatalogue::AddBus(const Domain::Bus& bus) {
        std::cout << "adding bus " << bus.bus_name_ << std::endl;
        buses_.emplace_back(bus);
        std::string_view bus_name_ = buses_.back().bus_name_;
        name_to_bus_[bus_name_] = &buses_.back();
        for (const Domain::Stop* stop : bus.bus_stops_) {
            const_cast<Domain::Stop*>(stop)->buses_for_stop_.emplace(&buses_.back());
        }
    }

    const Domain::Stop* TransportCatalogue::GetStopByName(std::string stop_name) const {
        return name_to_stops_.count(stop_name) ? name_to_stops_.at(stop_name) : (std::nullptr_t)nullptr;
    }

    const Domain::Bus* TransportCatalogue::GetBusByName(std::string bus_name) const {
        return name_to_bus_.count(bus_name) ? name_to_bus_.at(bus_name) : (std::nullptr_t)nullptr;
    }

    size_t TransportCatalogue::GetBusQty() {
        return buses_.size();
    }

    size_t TransportCatalogue::GetStopQty() {
        return stops_.size();
    }

} //namespace Transport