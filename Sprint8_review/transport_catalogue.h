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
		std::set<std::string> buses_for_stop_;
		geo::Coordinates coord_;
	};

	struct Route {
		int stops_count_;
		int unique_stops_count_=0;
		double route_length_=0.;
	};

	class TransportCatalogue {
		// Реализуйте класс самостоятельно
		public:
			void AddBus (const Bus& bus);
			void AddStop (const Stop& stop);
			const Bus* GetBusByName (const std::string& bus_name) const;
			const Stop* GetStopByName (const std::string& stop_name) const;
			const Route* GetRouteByName (const std::string& route_name) const;
			const std::set<std::string> GetRoutesForStop (const std::string& stop_name) const;
			void FillStopToRoute(const Bus& bus);
			void AddRoute(const Bus& bus);

		private:
			std::deque<Stop> stops_;
			std::deque<Bus> buses_;
			std::unordered_map <std::string_view, Stop*> name_to_stop_;
			std::unordered_map <std::string_view, Bus*> name_to_bus_;
			std::unordered_map <std::string_view, Route*> name_to_route_;
			std::unordered_map <std::string_view, std::set<std::string>> routes_for_stop_;
	};

} //namepace transport