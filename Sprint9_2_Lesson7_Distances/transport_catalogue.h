#pragma once

#include <deque>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "geo.h"

namespace transport {

	struct Bus {

		Bus() = default;

		Bus(const std::string& bus_name, const std::vector<std::string_view>& route) :
			route_name_(bus_name),
			route_stops_(route)
		{}
		std::string route_name_;
		std::vector<std::string_view> route_stops_;
	};

	struct Stop {

		Stop() = default;

		Stop (const std::string stop_name, const geo::Coordinates& coord) :
			stop_name_(stop_name),
			coord_(coord)
			{}
		std::string stop_name_;
		//std::set<std::string> buses_for_stop_;
		geo::Coordinates coord_;
	};

	struct BusInfo {

		BusInfo() = default;

		BusInfo(int stops_count) :
			stops_count_(stops_count)
		{}

		int stops_count_;
		int unique_stops_count_=0;
		int route_distance_=0;
		double route_length_=0;
		double curvature_=0.;
	};

	struct Key {
		const Stop* stop_from;
		const Stop* stop_to;
	};

	struct KeyEqual {
		using is_transparent = void;
		bool operator()(const Key& lhs, const Key& rhs) const {
			return lhs.stop_from == rhs.stop_from && lhs.stop_to == rhs.stop_to;
		}
	};

	struct KeyHasher {
		using is_transparent = void;
		using transparent_key_equal = KeyEqual;
		size_t operator()(const Key& stops) const {
			std::hash<std::string> hasher;
			return (hasher(stops.stop_from->stop_name_)*100 + hasher(stops.stop_to->stop_name_)*10);
		} 
	};

	class TransportCatalogue {
		// Реализуйте класс самостоятельно
		public:
			void AddBus (const Bus& bus);
			void AddStop (const Stop& stop);
			const Bus* GetBusByName (const std::string& bus_name) const;
			const Stop* GetStopByName (const std::string& stop_name) const;
			const BusInfo* GetRouteByName (const std::string& route_name) const;
			const std::set<std::string> GetRoutesForStop (const Stop* st_p) const;
			void FillStopToRoute(const Bus& bus);
			void AddBusInfo(const Bus& bus);
			void FillDistBwStops(Key, int dist);
			bool CheckIfContainsPair(Key& stops);
			int GetDistance (Key& stops);

		private:
			std::deque<Stop> stops_;
			std::deque<Bus> buses_;
			std::unordered_map <std::string_view, Stop*> name_to_stop_;
			std::unordered_map <std::string_view, Bus*> name_to_bus_;
			std::unordered_map <std::string_view, BusInfo*> name_to_route_;
			std::unordered_map <const Stop*, std::set<std::string>> routes_for_stop_;
			std::unordered_map <Key, int, KeyHasher, KeyEqual> distances_between_stops_;
	};

} //namepace transport