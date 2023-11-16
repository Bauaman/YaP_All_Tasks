#pragma once
#include <memory>
#include <string>
#include <variant>
#include <vector>
#include <set>
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

namespace Transport {

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
        //std::unordered_set<const Bus*> buses_for_stop_;
        //std::unordered_map<const Stop*, int> distances_;
        std::set<std::string_view> buses_for_stop_;
        size_t Hash() const {
            return std::hash<std::string>{}(stop_name_) + magic_*std::hash<double>{}(coord_.lat) +
                    magic_*magic_*std::hash<double>{}(coord_.lng);
        }

        static const size_t magic_{31};

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

    struct BusInfo {
		std::string bus_no_;
		int stops_count_=0;
		int unique_stops_count_=0;
		double route_distance_=0.;
		int route_length_=0.;
		double curvature_=0.;
	};

    class StatRequestOut {
    public:
        using Val = std::variant<std::nullptr_t, BusInfo*,const Stop*>;
        StatRequestOut() = default;
        StatRequestOut(BusInfo*);
        StatRequestOut(const Stop*);

        const Val& GetValue() const;
         
    private:
        Val value_;
    };

    using StopsPair = std::pair<const Stop*, const Stop*>;

}//namespace Transport

