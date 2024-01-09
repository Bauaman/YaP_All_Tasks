#pragma once

#include <iostream>
#include <tuple>

#include "domain.h"
#include "json.h"
#include "map_renderer.h"
#include "transport_router.h"
#include "transport_catalogue.h"

using namespace std::string_literals;

class JsonDataReader {
public:
    JsonDataReader(std::shared_ptr<TransportCatalogue> catalogue) :
        catalogue_(catalogue)
    {}

    void ProcessRequests(std::istream& input);
    void ProcessStatRequests(const std::shared_ptr<MapRenderer> renderer, const std::shared_ptr<TransportRouter> router);
    VisualizationSettings ProcessRenderSettings();
    RoutingSettings ProcessRoutingSettings();
    void PrintStatsAsJson(std::ostream& output);


private:
    void ParseAndProcessBaseRequests(const json::Node& node);
    //void ParseStatRequests(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);
    void BuildStatsForJson(const json::Node& node, const std::shared_ptr<MapRenderer> renderer, const std::shared_ptr<TransportRouter> router);

    void ParseStopBaseRequest(const json::Node& node);
    void ParseBusBaseRequest(const json::Node& node);

    json::Node FormBusResponceBuilder(const json::Node& node);
    json::Node FormStopResponceBuilder(const json::Node& node);
    json::Node FormMapResponceBuilder(const json::Node& node, const std::shared_ptr<MapRenderer> renderer);
    json::Node FormErrorResponceBuilder(int request_id);
    json::Node FormRouteResponceBuilder(const json::Node& node, const std::shared_ptr<TransportRouter> router);

    std::tuple<int, double, double/*, int*/> CaclRouteStats(Bus* bus);

    std::shared_ptr<TransportCatalogue> catalogue_;
    json::Array responces_;
    json::Array resp_json_builder_;
    json::Node root_node_;
};