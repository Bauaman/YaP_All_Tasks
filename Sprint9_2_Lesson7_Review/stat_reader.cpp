#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <stdexcept>

#include "input_reader.h"
#include "stat_reader.h"


namespace transport::readers {

    void StatRequestAndPrint (std::istream& input, std::ostream& output, TransportCatalogue& catalogue) {
        int stat_request_count;
        input >> stat_request_count >> std::ws;
        std::vector<std::string> lines;
        for (int i = 0; i < stat_request_count; ++i) {
            std::string line;
            getline(input, line);
            lines.push_back(line);
        }
        for (std::string& line : lines) {
            ParseAndPrintStat(catalogue, line, output);
        }
    }

    BusInfo ComputeBusInfo(const transport::Bus& bus, const transport::TransportCatalogue& tansport_catalogue) {
        BusInfo route;
        std::set<std::string_view> unique_stops;
        route.bus_no_ = bus.route_name_;
        route.stops_count_ = bus.route_stops_.size();
        for (size_t i=0; i<bus.route_stops_.size()-1; ++i) {
            const Stop* from = bus.route_stops_[i];
            const Stop* to = bus.route_stops_[i+1];
            transport::geo::Coordinates coord_from = from->coord_;
            transport::geo::Coordinates coord_to = to->coord_;
            route.route_length_ += transport::geo::ComputeDistance(coord_from, coord_to);
            route.route_distance_ += tansport_catalogue.GetDistance(from,to);
            unique_stops.emplace(bus.route_stops_[i]->stop_name_);
            route.curvature_ = route.route_distance_*1.0 / route.route_length_;
        }


            //const std::string stv_from = transport::readers::detail::AsString(bus.route_stops_[i]);
            //std::cout << std::endl << stv_from << std::endl;
            //const std::string stv_to = transport::readers::detail::AsString(bus.route_stops_[i+1]);
            //const Stop* from = tansport_catalogue.GetStopByName(stv_from); 
            //const Stop* to = tansport_catalogue.GetStopByName(stv_to);
            //transport::geo::Coordinates coord_from = from->coord_;
            //transport::geo::Coordinates coord_to = to->coord_;
            //route.route_length_ += transport::geo::ComputeDistance(coord_from, coord_to);
            //route.route_distance_ += tansport_catalogue.GetDistance(from,to);
            //unique_stops.emplace(bus.route_stops_[i]);
            //route.curvature_ = route.route_distance_*1.0 / route.route_length_;
        //}
            
        route.unique_stops_count_ = unique_stops.size();
        return route;
    }

    void PrintBus (const transport::Bus* bus, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue) {
            BusInfo route = ComputeBusInfo(*bus, tansport_catalogue);
            output << "Bus " << bus->route_name_ << ": " << route.stops_count_ 
                    << " stops on route, " << route.unique_stops_count_
                    << " unique stops, " << std::scientific << std::setprecision(5) << route.route_distance_
                     << " route length, " << std::fixed << std::setprecision(5) << route.curvature_
                     << " curvature\n";
    }

    void PrintStop (const transport::Stop* stop, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue) {

            const std::vector<const Bus*> res = tansport_catalogue.GetRoutesForStop(stop);
            if (res.size() > 0) {
                output << "Stop " << stop->stop_name_ << ": buses";
                for(const Bus* rt : res) {
                    output << " " << rt->route_name_;
                }
                output << "\n";
            } else {
                output << "Stop " << stop->stop_name_ << ": no buses\n";
            }

    }

    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                        std::ostream& output) {
        
        CommandDescription out_req = detail::ParseRequestCommand(request);
        if (out_req.command == "Bus") {
            const transport::Bus* found = tansport_catalogue.GetBusByName(out_req.id);
            if (found != (std::nullptr_t)nullptr) {
                PrintBus(found, output, tansport_catalogue);
            } else {
                output << "Bus " << out_req.id << ": not found\n";
            }       
        }
        if (out_req.command == "Stop") {
            const transport::Stop* found = tansport_catalogue.GetStopByName(out_req.id);
            if (found != (std::nullptr_t)nullptr) {
                PrintStop(found, output, tansport_catalogue);
            } else {
                output << "Stop " << out_req.id << ": not found\n";
            }

        }
    }

} //namespace readers