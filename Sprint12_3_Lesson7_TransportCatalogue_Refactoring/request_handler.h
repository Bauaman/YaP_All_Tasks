#pragma once

#include <iostream>
#include "json_reader.h"
#include "map_renderer.h"
#include "svg.h"
#include "transport_catalogue.h"
#include "transport_router.h"

class RequestHandler {
public:
    RequestHandler(TransportCatalogue& tc) :
        catalogue_(std::make_shared<TransportCatalogue>(tc))
    {
        GetHandler();
    }

    ~RequestHandler() = default;

    void ProcessRequests(std::istream& input);
    void ProcessStatRequests();
    void PrintStatsJson(std::ostream& out);
    void LinkTransportCatalogue(TransportCatalogue& tc);

    void RenderMap();

    const std::optional<graph::Router<double>::RouteInfo> GetOptimalRoute(const std::string_view stop_from, const std::string_view stop_to) const;
    const graph::DirectedWeightedGraph<double>& GetRouterGraph();

private:
    void GetHandler();
    void GetMapRenderer();
    void GetTransportRouter();
    std::shared_ptr<TransportCatalogue> catalogue_;
    //VisualizationSettings render_settings_;
    std::shared_ptr<JsonDataReader> handler_;
    std::shared_ptr<MapRenderer> renderer_;
    std::shared_ptr<TransportRouter> router_;
};