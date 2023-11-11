#pragma once
#include <deque>
#include <memory>
#include <string_view>
#include <unordered_map>

#include "domain.h"
#include "json.h"

namespace Transport {

    class TransportCatalogue {
    public:
        TransportCatalogue() = default;

        size_t GetBusQty();
        size_t GetStopQty();

        void AddStop(const Domain::Stop* stop);
        void AddBus(const Domain::Bus& bus);
        const Domain::Stop* GetStopByName(std::string stop_name) const;
        const Domain::Bus* GetBusByName(std::string bus_name) const;
    private:
        std::deque<Domain::Stop> stops_;
        std::deque<Domain::Bus> buses_;
        std::unordered_map<std::string_view, const Domain::Stop*> name_to_stops_;
        std::unordered_map<std::string_view, const Domain::Bus*> name_to_bus_;
    };

} //namespace Transport