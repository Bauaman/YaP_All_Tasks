#include "transport_router.h"

graph::DirectedWeightedGraph<double> TransportRouter::BuildGraph(const TransportCatalogue& catalogue) {
    const auto& sorted_stops = catalogue.GetSortedStops();
    const auto& sorted_buses = catalogue.GetSortedBuses();
    graph::VertexId vertex_id = 0;
    std::unordered_map<std::string_view, graph::VertexId> stop_ids;
    graph::DirectedWeightedGraph<double> stops_graph(sorted_stops.size()*2);

    for (const auto& [stop_name, stop_info] : sorted_stops) {
        stop_ids.emplace(std::make_pair(stop_name, vertex_id));
        stops_graph.AddEdge({
            stop_info->stop_name_,
            0,
            vertex_id,
            ++vertex_id,
            static_cast<double>(bus_wait_time_)
        });
        ++vertex_id;
    }

    stop_ids_ = std::move(stop_ids);
    
    for (const auto& [bus_name, bus_info] : sorted_buses) {
        const auto& bus_stops = bus_info->route_stops_;
        size_t stops_count = bus_stops.size();
        for (size_t i=0; i < stops_count; ++i) {
            for (size_t j=i+1; j < stops_count; ++j) {
                const Stop* stop_from = bus_stops[i];
                const Stop* stop_to = bus_stops[j];
                int dist_sum = 0;
                int dist_sum_reverse = 0;
                for (size_t k=i+1; k<=j; ++k) {
                    dist_sum += catalogue.GetDistanceBetweenStops(bus_stops[k-1], bus_stops[k]);
                    dist_sum_reverse += catalogue.GetDistanceBetweenStops(bus_stops[k], bus_stops[k-1]);
                }
                stops_graph.AddEdge({
                    bus_info->bus_name_,
                    j-i,
                    stop_ids_.at(stop_from->stop_name_) +1,
                    stop_ids_.at(stop_to->stop_name_),
                    static_cast<double>(dist_sum)/(bus_velocity_*(100./6.))
                });
                if (!bus_info->is_round_) {
                    stops_graph.AddEdge({
                        bus_info->bus_name_,
                        j-i,
                        stop_ids_.at(stop_to->stop_name_) +1,
                        stop_ids_.at(stop_from->stop_name_),
                        static_cast<double>(dist_sum_reverse)/(bus_velocity_*(100./6.))
                    });
                }
            }
        }
    }

    graph_ = std::move(stops_graph);
    router_ = std::make_unique<graph::Router<double>>(graph_);

    return graph_;
}

const std::optional<graph::Router<double>::RouteInfo> TransportRouter::FindRoute(const std::string_view from, const std::string_view to) const {
    if (!stop_ids_.count(std::string(from)) || !stop_ids_.count(std::string(to))) {
        return std::nullopt;
    }
    return router_->BuildRoute(stop_ids_.at(std::string(from)), stop_ids_.at(std::string(to)));
}

const graph::DirectedWeightedGraph<double>& TransportRouter::GetGraph() const {
    return graph_;
}