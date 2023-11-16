#include "json_reader.h"

void JsonDataReader::ProcessRequests(std::istream& input) {
    json::Node root_node = json::Load(input).GetRoot().AsDict();
    const json::Node& basic_req_node = root_node.AsDict().at("base_requests").AsArray();
    ParseAndProcessBaseRequests(basic_req_node);
    const json::Node& stat_req_node = root_node.AsDict().at("stat_requests").AsArray();
    ParseStatRequests(stat_req_node);
}

void JsonDataReader::ParseAndProcessBaseRequests(const json::Node& node) {
    std::vector<json::Node> bus_basic_reqs;
    for (auto& nd : node.AsArray()) {
        if (nd.AsDict().at("type").AsString() == "Stop") {
            ParseStopBaseRequest(nd);
        }
        if (nd.AsDict().at("type").AsString() == "Bus") {
            bus_basic_reqs.push_back(nd);
        }
    }
    for (const json::Node& bus_req : bus_basic_reqs) {
        ParseBusBaseRequest(bus_req);
    }
    ProcessStopsPairDistances();
}

void JsonDataReader::ParseStopBaseRequest(const json::Node& node) {
    auto stop_node = node.AsDict();
    std::string st_name = stop_node.at("name").AsString();
    geo::Coordinates st_coord = {stop_node.at("latitude").AsDouble(),
                               stop_node.at("longitude").AsDouble()};
    Stop stop(st_name, st_coord);
    for (auto& dist_pair : stop_node.at("road_distances").AsDict()) {
        stop.dist_to_stops_[dist_pair.first] = dist_pair.second.AsInt();
    }
    catalogue_->AddStop(std::move(stop));
}

void JsonDataReader::ParseBusBaseRequest(const json::Node& node) {
    auto bus_node = node.AsDict();
    std::string bs_name = bus_node.at("name").AsString();
    //RouteType bs_type = RouteType::NOT_ROUND;
    bool is_round_trip = false;
    if (bus_node.at("is_roundtrip").AsBool()) {
        is_round_trip = true;
    }
    Bus bus(bs_name, is_round_trip);
    for (auto& st_ : bus_node.at("stops").AsArray()) {
        Stop* curr_stop = catalogue_->GetStopByName(st_.AsString());
        bus.route_stops_.push_back(curr_stop);
        bus.unique_stops_.emplace(curr_stop);
    }
    catalogue_->AddBus(std::move(bus));
}

void JsonDataReader::ProcessStopsPairDistances() {
    catalogue_->SetDistancesBetweenStops();
}

void JsonDataReader::ParseStatRequests(const json::Node& node) {
    for (auto& request : node.AsArray()) {
        auto req_ = request.AsDict();
        if (req_.at("type").AsString() == "Bus") {
            responces_.emplace_back(FormBusResponce(req_));
        } else if (req_.at("type").AsString() == "Stop") {
            responces_.emplace_back(FormStopResponce(req_));
        }
    }
}

json::Node JsonDataReader::FormBusResponce(const json::Node& node) {
    json::Dict responce;
    BusInfo bus_info;
    bus_info.request_id_ = node.AsDict().at("id").AsInt();
    bus_info.bus_no_ = node.AsDict().at("name").AsString();
    auto bus = catalogue_->GetBusByName(bus_info.bus_no_);
    if (!bus) {
        return FormErrorResponce(bus_info.request_id_);
    }
    bus_info.stops_count_ = bus->GetStopsCount();
    
    std::tuple<int, double, double/*, int*/> stat = CaclRouteStats(bus);
    bus_info.unique_stops_count_ = bus->GetUniqueStopsCount();
    bus_info.route_length_ = std::get<0>(stat);
    bus_info.route_geo_distance = std::get<1>(stat);
    bus_info.curvature = std::get<2>(stat);

    responce.emplace("request_id", bus_info.request_id_);
    responce.emplace("curvature", bus_info.curvature);
    responce.emplace("route_length", bus_info.route_length_);
    responce.emplace("stop_count", bus_info.stops_count_);
    responce.emplace("unique_stop_count", bus_info.unique_stops_count_);
    return responce;
}

json::Node JsonDataReader::FormStopResponce(const json::Node& node) {
    json::Array buses;
    json::Dict responce;
    Stop* stop = catalogue_->GetStopByName(node.AsDict().at("name").AsString());
    if (!stop) {
        return FormErrorResponce(node.AsDict().at("id").AsInt());
    }
    for (auto& bn : catalogue_->GetRoutesForStop(stop)) {
        //auto bus = catalogue_->GetBusByName(bn);
        std::string bns(bn);
        buses.push_back(bns);
    }
    responce.emplace("request_id", node.AsDict().at("id").AsInt());
    responce.emplace("buses", buses);
    return responce;
}

json::Node JsonDataReader::FormErrorResponce(int request_id) {
    json::Dict responce;
    responce.emplace("request_id", request_id);
    responce.emplace("error_message"s, "not found"s);
    return responce;
}

std::tuple<int, double, double/*, int*/> JsonDataReader::CaclRouteStats(Bus* bus) {
    int len=0; 
    double geo_dist=0., curv=0.;
    //std::unordered_set<Stop*> unique_stops;
    for (size_t i = 0; i < bus->route_stops_.size() - 1; ++i ) {
        Stop* from = bus->route_stops_[i];
        Stop* to = bus->route_stops_[i+1];
        len += catalogue_->GetDistanceBetweenStops(from,to);
        geo_dist += geo::ComputeDistance(from->coord_,to->coord_);
        //unique_stops.emplace(from);
        //unique_stops.emplace(to);
    }
    curv = len *1. / geo_dist;
    return {len, geo_dist, curv};//, unique_stops.size()};
}

void JsonDataReader::PrintStatsAsJson(std::ostream& output) {
    json::Print(responces_, output);
}