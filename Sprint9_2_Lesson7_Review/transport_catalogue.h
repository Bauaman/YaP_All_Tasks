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

	struct Stop {

		Stop() = default;

		Stop (const std::string stop_name, const geo::Coordinates& coord) :
			stop_name_(stop_name),
			coord_(coord)
			{}
		std::string stop_name_;
		geo::Coordinates coord_;
	};

	struct Bus {

		Bus() = default;

		Bus(const std::string& bus_name, std::vector<const Stop*> route) :
			route_name_(bus_name),
			route_stops_(route)
		{
		}

		std::string route_name_;
		std::vector<const Stop*> route_stops_;

	};

	struct BusInfo {
		std::string bus_no_;
		int stops_count_;
		int unique_stops_count_=0;
		double route_distance_=0.;
		double route_length_=0.;
		double curvature_=0.;
	};

	struct StopsPair {
		const Stop* stop_from;
		const Stop* stop_to;

		bool operator==(const StopsPair& other) const {
			return stop_from == other.stop_from && stop_to == other.stop_to;
		}
	};

	struct StopsPairHasher {
		size_t operator()(const StopsPair& stops) const {
			std::hash<std::string> hasher;
			return (hasher(stops.stop_from->stop_name_)*100 + hasher(stops.stop_to->stop_name_)*10);
		}
	};

	struct compareBuses {
		bool operator()(const Bus* bus_left, const Bus* bus_right) const {
			return bus_left->route_name_ < bus_right->route_name_;
		}
	};

	class TransportCatalogue {
		// Реализуйте класс самостоятельно
		public:
			void AddBus (const Bus& bus);
			void AddStop (const Stop& stop);
			const Bus* GetBusByName (const std::string& bus_name) const;
			const Stop* GetStopByName (const std::string& stop_name) const;
			const std::vector<const Bus*> GetRoutesForStop (const Stop* st_p) const;

			int GetDistance (const Stop* from, const Stop* to) const;
			void SetDistance(const Stop* from, const Stop* to, int dist);

		private:
			std::deque<Stop> stops_;
			std::deque<Bus> buses_;
			std::unordered_map <std::string_view, Stop*> name_to_stop_;
			std::unordered_map <std::string_view, Bus*> name_to_bus_;
			std::unordered_map <const Stop*,std::unordered_set<const Bus*>> routes_for_stop_;
			std::unordered_map <StopsPair, int, StopsPairHasher/*, KeyEqual*/> distances_between_stops_;
			

	};

} //namepace transport