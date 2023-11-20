#pragma once

#include <iostream>
#include "json_reader.h"
#include "map_renderer.h"
#include "svg.h"
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
    void ProcessStatRequests();
    void PrintStatsJson(std::ostream& out);
    void LinkTransportCatalogue(TransportCatalogue& tc);

    void RenderMap();
    

private:
    void GetHandler();
    void GetMapRenderer();
    TransportCatalogue* catalogue_;
    //VisualizationSettings render_settings_;
    JsonDataReader* handler_;
    MapRenderer* renderer_;
};