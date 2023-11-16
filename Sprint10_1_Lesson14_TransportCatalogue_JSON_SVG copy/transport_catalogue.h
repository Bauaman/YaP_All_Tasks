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

        void AddStop(const Stop* stop);
        void AddBus(const Bus& bus);
        int GetDistance (const Stop* from, const Stop* to) const;
        void SetDistance (const Stop* from, const Stop* to, int dist);
        const Transport::Stop* GetStopByName(std::string_view stop_name) const;
        const Transport::Bus* GetBusByName(std::string_view bus_name) const;

    private:
        struct StopsPairHash {
            size_t operator()(const StopsPair& pair) const {
                return pair.first->Hash() + prime_num * pair.second->Hash();
            }
        

        static const size_t prime_num{47};
        };
    private:
        static const size_t prime_number{31};

    private:

        std::deque<Stop> stops_;
        std::deque<Bus> buses_;
        std::unordered_map<std::string_view, const Stop*> name_to_stops_;
        std::unordered_map<std::string_view, const Bus*> name_to_bus_;
        std::unordered_map<StopsPair, int, StopsPairHash> distances_between_stops_;
    };

} //namespace Transport