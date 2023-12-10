#pragma once

#include <iostream>
#include <tuple>

#include "domain.h"
#include "json.h"
#include "map_renderer.h"
#include "transport_catalogue.h"

using namespace std::string_literals;

class JsonDataReader {
public:
    JsonDataReader(std::shared_ptr<TransportCatalogue> catalogue) :
        catalogue_(catalogue)
    {}

    void ProcessRequests(std::istream& input);
    void ProcessStatRequests(const std::shared_ptr<MapRenderer> renderer);
    VisualizationSettings ProcessRenderSettings();
    void PrintStatsAsJson(std::ostream& output);


private:
    void ParseAndProcessBaseRequests(const json::Node& node);
    void ParseStatRequests(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);
    void BuildStatsForJson(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);

    void ParseStopBaseRequest(const json::Node& node);
    void ParseBusBaseRequest(const json::Node& node);

    
    json::Node FormBusResponce(const json::Node& node);
    json::Node FormStopResponce(const json::Node& node);
    json::Node FormMapResponce(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);
    json::Node FormErrorResponce(int request_id);


    json::Node FormBusResponceBuilder(const json::Node& node);
    json::Node FormStopResponceBuilder(const json::Node& node);
    json::Node FormMapResponceBuilder(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);
    json::Node FormErrorResponceBuilder(int request_id);

    std::tuple<int, double, double/*, int*/> CaclRouteStats(Bus* bus);

    std::shared_ptr<TransportCatalogue> catalogue_;
    json::Array responces_;
    json::Array resp_json_builder_;
    json::Node root_node_;
};