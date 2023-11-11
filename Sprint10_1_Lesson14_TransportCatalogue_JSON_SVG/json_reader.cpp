
#include "json_reader.h"

void JsonDataReader::ProcessInputRequests(Inputer* input) {
    auto root = json::Load(input->GetStream()).GetRoot();
    json::Dict root_node = root.AsDict();
    for (auto& [name, node] : root_node) {
        std::cout << name << std::endl;
    } 
    const json::Node& basic_req_node = root_node.at("base_requests").AsArray();
    ParseBaseRequests(basic_req_node);
}

void JsonDataReader::ParseBaseRequests(const json::Node& node) {
    std::vector<const json::Node*> bus_basic_reqs;
    for (auto& nd : node.AsArray()) {
        if (nd.AsDict().at("type").AsString() == "Stop") {
            AddStopToCatalogue(nd);
        }
        if (nd.AsDict().at("type").AsString() == "Bus") {
            bus_basic_reqs.push_back(&nd);
        }
    }
    for (const json::Node* bus_req : bus_basic_reqs) {
        AddBusToCatalogue(bus_req);
    }
    for (auto& nd : node.AsArray()) {
        if (nd.AsDict().at("type").AsString() == "Stop") {
            SetDistancesForStopInCatalogue(&nd);
        }
    }
}

void JsonDataReader::AddStopToCatalogue(const json::Node& node) {
    Domain::Stop stop(node.AsDict().at("name").AsString(), 
                      {node.AsDict().at("latitude").AsDouble(),
                      node.AsDict().at("longitude").AsDouble()});
    transport_catalogue_->AddStop(std::move(stop));    
}

void JsonDataReader::AddBusToCatalogue(const json::Node* node) {
    std::vector<const Domain::Stop*> stops;
    for (auto& stop_name : node->AsDict().at("stops").AsArray()) {
        stops.push_back(transport_catalogue_->GetStopByName(stop_name.AsString()));
    }
    Domain::Bus bus(node->AsDict().at("name").AsString(),
                    std::move(stops),
                    node->AsDict().at("is_roundtrip").AsBool());
    transport_catalogue_->AddBus(std::move(bus));
}

void JsonDataReader::SetDistancesForStopInCatalogue(const json::Node* node){
    Domain::Stop* curr_stop = const_cast<Domain::Stop*>(transport_catalogue_->GetStopByName(node->AsDict().at("name").AsString()));
    for (auto& [stop_name, dist] : node->AsDict().at("road_distances").AsDict()) {
        const Domain::Stop* dest_stop = transport_catalogue_->GetStopByName(stop_name);
        curr_stop->distances_.emplace(dest_stop, dist.AsDouble());
    }
}