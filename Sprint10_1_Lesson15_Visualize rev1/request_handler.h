#pragma once

#include <iostream>
#include "json_reader.h"
#include "map_renderer.h"
#include "svg.h"
#include "transport_catalogue.h"

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
    

private:
    void GetHandler();
    void GetMapRenderer();
    std::shared_ptr<TransportCatalogue> catalogue_;
    //VisualizationSettings render_settings_;
    std::shared_ptr<JsonDataReader> handler_;
    std::shared_ptr<MapRenderer> renderer_;
};