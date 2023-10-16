#include <algorithm>
#include <iostream>
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

    void PrintBus (std::string& request, const transport::Route* route, std::ostream& output) {
        if (route != (std::nullptr_t)nullptr) {
            output << "Bus " << request << ": " << route->stops_count_ 
                    << " stops on route, " << route->unique_stops_count_
                    << " unique stops, " << route->route_length_
                     << " route length\n";
        } else {
            output << "Bus " << request << ": not found\n";
        }
    }

    void PrintStop (std::string& request, const transport::Stop* stop, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue) {
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
        try {
            if (tansport_catalogue.GetRoutesForStop(request).size() > 0) {
                output << "Stop " << request << ": buses";
                for(const std::string& rt : tansport_catalogue.GetRoutesForStop(request)) {
                    output << " " << rt;
                }
                output << "\n";
            } else {
                output << "Stop " << request << ": no buses\n";
            }
        }
        catch(const std::exception& e)
        {
            output << "Stop " << request << ": not found\n";
        }
    }

    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                        std::ostream& output) {
        // Реализуйте самостоятельно
        
        CommandDescription out_req = detail::ParseRequestCommand(request);
        if (out_req.command == "Bus") {
            const transport::Route* found = tansport_catalogue.GetRouteByName(out_req.id);
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
            PrintStop(out_req.id, found, output, tansport_catalogue);
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