#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <stdexcept>

#include "stat_reader.h"
#include "input_reader.h"

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

    void PrintBus (std::string& request, const transport::BusInfo* route, std::ostream& output) {
        if (route != (std::nullptr_t)nullptr) {
            output << "Bus " << request << ": " << route->stops_count_ 
                    << " stops on route, " << route->unique_stops_count_
                    << " unique stops, " << route->route_distance_
                     << " route length, " << route->curvature_
                     << " curvature\n";
        } else {
            output << "Bus " << request << ": not found\n";
        }
    }

    void PrintStop (const transport::Stop* stop, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue) {
        /*
        if (stop != (std::nullptr_t)nullptr) {
            if (stop->buses_for_stop_.size() > 0) {
                output << "Stop " << request << ": buses";
                for (auto& str : stop->buses_for_stop_) {
                    output << " " << str;
                }
                output << "\n";
            }
            else {
                output << "Stop " << request << ": no buses\n";
            }
        }
        else {
            output << "Stop " << request << ": not found\n";
        }*/

        //try {
            const std::set<std::string> res = tansport_catalogue.GetRoutesForStop(stop);
            if (res.size() > 0) {
                output << "Stop " << stop->stop_name_ << ": buses";
                for(const std::string& rt : res) {
                    output << " " << rt;
                }
                output << "\n";
            } else {
                output << "Stop " << stop->stop_name_ << ": no buses\n";
            }

    }

    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                        std::ostream& output) {
        // Реализуйте самостоятельно
        
        CommandDescription out_req = detail::ParseRequestCommand(request);
        if (out_req.command == "Bus") {
            const transport::BusInfo* found = tansport_catalogue.GetRouteByName(out_req.id);
            PrintBus(out_req.id, found, output);
            /*
            if (found != (std::nullptr_t)nullptr) {
                output << "Bus " << out_req.id << ": " << found->stops_count_ 
                        << " stops on route, " << found->unique_stops_count_
                        << " unique stops, " << found->route_length_
                        << " route length\n";
            } else {
                output << "Bus " << out_req.id << ": not found\n";
            }*/
        }
        if (out_req.command == "Stop") {
            const transport::Stop* found = tansport_catalogue.GetStopByName(out_req.id);
            if (found != (std::nullptr_t)nullptr) {
                PrintStop(found, output, tansport_catalogue);
            } else {
                output << "Stop " << out_req.id << ": not found\n";
            }
           /*
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
            */
        }
    }

} //namespace readers