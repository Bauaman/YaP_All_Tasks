#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace transport::readers {

    void StatRequestAndPrint (std::istream& input, std::ostream& output, TransportCatalogue& catalogue);
    std::unique_ptr<BusInfo> ComputeBusInfo(const Bus& bus, const transport::TransportCatalogue& tansport_catalogue);
    void PrintBus (std::string& request, const transport::BusInfo* route, std::ostream& output);
    void PrintStop (const transport::Stop* stop, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue);
    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                            std::ostream& output);

}