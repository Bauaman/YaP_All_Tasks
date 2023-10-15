#include <algorithm>
#include <iostream>

#include "stat_reader.h"
#include "input_reader.h"

namespace readers {

    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                        std::ostream& output) {
        // Реализуйте самостоятельно
        
        CommandDescription out_req = ParseRequestCommand(request);
        if (out_req.command == "Bus") {
            const transport::Route* found = tansport_catalogue.GetRouteByName(out_req.id);
            if (found != (std::nullptr_t)nullptr) {
                output << "Bus " << out_req.id << ": " << found->stops_count_ 
                        << " stops on route, " << found->unique_stops_count_
                        << " unique stops, " << found->route_length_
                        << " route length\n";
            } else {
                output << "Bus " << out_req.id << ": not found\n";
            }
        }
        if (out_req.command == "Stop") {
            const transport::Stop* found = tansport_catalogue.GetStopByName(out_req.id);
            if (found != (std::nullptr_t)nullptr) {
                if (found->buses_for_stop_.size() > 0) {
                    output << "Stop " << out_req.id << ": buses";
                    for (auto& str : found->buses_for_stop_) {
                        output << " " << str;
                    }
                    output << "\n";
                }
                else {
                    output << "Stop " << out_req.id << ": no buses\n";
                }
            }
            else {
                output << "Stop " << out_req.id << ": not found\n";
            }
        }
    }

} //namespace readers