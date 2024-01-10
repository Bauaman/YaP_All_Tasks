#include "transport_router.h"

graph::DirectedWeightedGraph<double> TransportRouter::BuildGraph(const TransportCatalogue& catalogue) {   
    FillVertexes(catalogue);
    graph_ = graph::DirectedWeightedGraph<double>(stop_ids_.size());
    FillEdges(catalogue);
    router_ = std::make_unique<graph::Router<double>>(graph_);
    return graph_;
}

void TransportRouter::FillVertexes(const TransportCatalogue& catalogue) {
    const auto& sorted_stops = catalogue.GetSortedStops();
    size_t vertex_id = 0;
    for (auto& [stop_name, stop] : sorted_stops) {
        stop_ids_.emplace(std::make_pair(stop_name, vertex_id));
        vertex_by_id_.emplace(std::make_pair(vertex_id, stop_name));
        ++vertex_id;
    }
}

void TransportRouter::FillEdges(const TransportCatalogue& catalogue) {
    const auto& sorted_buses = catalogue.GetSortedBuses();
    for (auto& [bus_name, bus] : sorted_buses) {
        std::vector<double> dist_sums_forw(bus->route_stops_.size());
        std::vector<double> dist_sums_rev(bus->route_stops_.size());
        int dist_sum = 0;
        int dist_sum_reverse = 0;
        for (size_t index = 1; index < bus->route_stops_.size(); index++) {
            dist_sum += catalogue.GetDistanceBetweenStops(bus->route_stops_.at(index-1), bus->route_stops_.at(index));
            dist_sums_forw[index] = dist_sum;
            dist_sum_reverse += catalogue.GetDistanceBetweenStops(bus->route_stops_.at(index), bus->route_stops_.at(index-1));
            dist_sums_rev[index] = dist_sum_reverse;
        }
        for (int i =0 ; i+1 < bus->route_stops_.size(); ++i) {
            for (int j = i+1; j < bus->route_stops_.size(); ++j) {
                RouteMember r_member;
                r_member.start_index = bus->route_stops_.at(i)->stop_name_;
                r_member.finish_index = bus->route_stops_.at(j)->stop_name_;
                r_member.route_info = bus;
                r_member.stops_count = std::abs(i-j);
                r_member.wait_time = bus_wait_time_;
                r_member.trip_time = (i<j ? dist_sums_forw[j] - dist_sums_forw[i] : dist_sums_rev[i] - dist_sums_rev[j]) / (bus_velocity_*100./6.);
                graph::EdgeId edge_id = graph_.AddEdge(graph::Edge<double>{
                        r_member.route_info->bus_name_,
                        r_member.stops_count,
                        stop_ids_.at(r_member.start_index),
                        stop_ids_.at(r_member.finish_index),
                        r_member.trip_time + r_member.wait_time
                    });
                if (!bus->is_round_) {
                    //RouteMember r_member;
                    r_member.start_index = bus->route_stops_.at(j)->stop_name_;
                    r_member.finish_index = bus->route_stops_.at(i)->stop_name_;
                    //r_member.route_info = bus;
                    //r_member.stops_count = std::abs(i-j);
                    //r_member.wait_time = bus_wait_time_;
                    r_member.trip_time = (j<i ? dist_sums_forw[i] - dist_sums_forw[j] : dist_sums_rev[j] - dist_sums_rev[i]) / (bus_velocity_*100./6.);
                    graph::EdgeId edge_id = graph_.AddEdge(graph::Edge<double>{
                            r_member.route_info->bus_name_,
                            r_member.stops_count,
                            stop_ids_.at(r_member.start_index),
                            stop_ids_.at(r_member.finish_index),
                            r_member.trip_time + r_member.wait_time
                        });
                }
            }
        }

    }
}

/*

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

*/

const int TransportRouter::GetWaitTime() const {
    return bus_wait_time_;
}

const std::string TransportRouter::GetVertexById(graph::VertexId id) const {
    return std::string(vertex_by_id_.at(id));
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