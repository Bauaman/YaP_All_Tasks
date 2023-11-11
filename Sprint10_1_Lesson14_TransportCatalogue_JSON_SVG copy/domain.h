#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include "geo.h"
/*
 * В этом файле вы можете разместить классы/структуры, которые являются частью предметной области (domain)
 * вашего приложения и не зависят от транспортного справочника. Например Автобусные маршруты и Остановки. 
 *
 * Их можно было бы разместить и в transport_catalogue.h, однако вынесение их в отдельный
 * заголовочный файл может оказаться полезным, когда дело дойдёт до визуализации карты маршрутов:
 * визуализатор карты (map_renderer) можно будет сделать независящим от транспортного справочника.
 *
 * Если структура вашего приложения не позволяет так сделать, просто оставьте этот файл пустым.
 *
 */

namespace Domain {

//    struct Stop;
    struct Bus;

    struct Stop {
    public:
        Stop() = default;
        Stop(const std::string name,const geo::Coordinates& coords) :
            stop_name_(name),
            coord_(coords)
        {}

        const std::string stop_name_;
        const geo::Coordinates coord_;
        std::unordered_set<const Bus*> buses_for_stop_;
        std::unordered_map<const Stop*, int> distances_;
    };

    struct Bus {
    public:
        Bus() = default;
        Bus(std::string name, std::vector<const Stop*> stops, bool is_roundtrip) :
            bus_name_(name),
            bus_stops_(stops),
            is_round_(is_roundtrip)
        {}

        std::string bus_name_;
        std::vector<const Stop*> bus_stops_;
        bool is_round_;
    };

}//namespace Domain

