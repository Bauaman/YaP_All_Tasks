#pragma once

#include "graph.h"
#include "router.h"
#include "transport_catalogue.h"

#include <algorithm>
#include <memory>

struct RoutingSettings {
    int bus_velocity = 0;
    int bus_wait_time = 0;
};

class TransportRouter {
public: 
    TransportRouter(const RoutingSettings& settings) :
        bus_velocity_(settings.bus_velocity),
        bus_wait_time_(settings.bus_wait_time)
    {}
    

    graph::DirectedWeightedGraph<double> BuildGraph(const TransportCatalogue& catalogue);
    const std::optional<graph::Router<double>::RouteInfo> FindRoute(const std::string_view from, const std::string_view to) const;
    const graph::DirectedWeightedGraph<double>& GetGraph() const;
private:
    int bus_velocity_ = 0;
    int bus_wait_time_ = 0;

    graph::DirectedWeightedGraph<double> graph_;
    std::unordered_map<std::string_view, graph::VertexId> stop_ids_;
    std::unique_ptr<graph::Router<double>> router_;
};