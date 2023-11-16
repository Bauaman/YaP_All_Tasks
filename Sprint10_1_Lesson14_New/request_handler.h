#pragma once

#include <iostream>
#include "json_reader.h"
#include "transport_catalogue.h"

class RequestHandler {
public:
    RequestHandler(TransportCatalogue& tc) :
        catalogue_(&tc)
    {
        GetHandler();
    }

    ~RequestHandler() = default;

    void ProcessRequests(std::istream& input);
    void PrintStatsJson(std::ostream& out);
    void LinkTransportCatalogue(TransportCatalogue& tc);

private:
    void GetHandler();
    TransportCatalogue* catalogue_;
    JsonDataReader* handler_;
};