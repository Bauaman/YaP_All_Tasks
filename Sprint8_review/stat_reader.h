#pragma once

#include <iosfwd>
#include <string_view>

#include "transport_catalogue.h"

namespace transport::readers {

    void StatRequestAndPrint (std::istream& input, std::ostream& output, TransportCatalogue& catalogue);

    void PrintBus (std::string& request, const transport::Route* route, std::ostream& output);
    void PrintStop (std::string& request, const transport::Stop* stop, std::ostream& output, const transport::TransportCatalogue& tansport_catalogue);
    void ParseAndPrintStat(const transport::TransportCatalogue& tansport_catalogue, std::string_view request,
                            std::ostream& output);

}