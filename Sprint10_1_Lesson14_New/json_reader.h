#pragma once

#include <iostream>
#include <tuple>

#include "domain.h"
#include "json.h"
#include "transport_catalogue.h"

using namespace std::string_literals;

class JsonDataReader {
public:
    JsonDataReader(TransportCatalogue* catalogue) :
        catalogue_(catalogue)
    {}

    void ProcessRequests(std::istream& input);
    void PrintStatsAsJson(std::ostream& output);

private:
    void ParseAndProcessBaseRequests(const json::Node& node);
    void ParseStatRequests(const json::Node& node);

    void ParseStopBaseRequest(const json::Node& node);
    void ParseBusBaseRequest(const json::Node& node);
    void ProcessStopsPairDistances();
    
    json::Node FormBusResponce(const json::Node& node);
    json::Node FormStopResponce(const json::Node& node);
    json::Node FormErrorResponce(int request_id);

    std::tuple<int, double, double/*, int*/> CaclRouteStats(Bus* bus);

    TransportCatalogue* catalogue_;
    json::Array responces_;
};