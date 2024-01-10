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

struct RouteMember {
    RouteMember() = default;

    std::string_view start_index;
    std::string_view finish_index;
    const Bus* route_info = nullptr;

    int stops_count = 0;
    double trip_time = 0;
    double wait_time = 0;
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

    const int GetWaitTime() const;
    const std::string GetVertexById(graph::VertexId id) const;

private:
    int bus_velocity_ = 0;
    int bus_wait_time_ = 0;

    void FillVertexes(const TransportCatalogue& catalogue);
    void FillEdges(const TransportCatalogue& catalogue);

    graph::DirectedWeightedGraph<double> graph_;
    std::unordered_map<std::string_view, graph::VertexId> stop_ids_;
    std::unordered_map<graph::VertexId, std::string_view> vertex_by_id_;

    std::unique_ptr<graph::Router<double>> router_;
};