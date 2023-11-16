
#include "json_reader.h"

using namespace std::string_literals;

void JsonDataReader::ProcessInputRequests(Inputer* input) {
    json::Dict root_node = json::Load(input->GetStream()).GetRoot().AsDict();
    const json::Node& basic_req_node = root_node.at("base_requests").AsArray();
    ParseBaseRequests(&basic_req_node);
    const json::Node& stat_req_node = root_node.at("stat_requests").AsArray();
    ParseStatRequests(&stat_req_node);
}

void JsonDataReader::ParseBaseRequests(const json::Node* node) {
    std::vector<const json::Node*> bus_basic_reqs;
    for (auto& nd : node->AsArray()) {
        if (nd.AsDict().at("type").AsString() == "Stop") {
            AddStopToCatalogue(&nd);
        }
        if (nd.AsDict().at("type").AsString() == "Bus") {
            bus_basic_reqs.push_back(&nd);
        }
    }
    for (auto& nd : node->AsArray()) {
        if (nd.AsDict().at("type").AsString() == "Stop") {
            SetDistancesForStopInCatalogue(&nd);
        }
    }
    for (const json::Node* bus_req : bus_basic_reqs) {
        AddBusToCatalogue(bus_req);
    }
}

void JsonDataReader::AddStopToCatalogue(const json::Node* node) {
    Transport::Stop stop(node->AsDict().at("name").AsString(), 
                      {node->AsDict().at("latitude").AsDouble(),
                      node->AsDict().at("longitude").AsDouble()});
    catalogue_->AddStop(&stop);    
}

void JsonDataReader::AddBusToCatalogue(const json::Node* node) {
    std::vector<const Transport::Stop*> stops;

    for (auto& stop_name : node->AsDict().at("stops").AsArray()) {
        stops.push_back(catalogue_->GetStopByName(stop_name.AsString()));
    }
    if (node->AsDict().at("is_roundtrip").AsBool()) {
        const Transport::Stop* temp = stops.at(0);
        stops.emplace_back(temp); 
    } else {
        int tempsize = stops.size()-2;
        while (tempsize >= 0)
        {
            const Transport::Stop* temp = stops.at(tempsize);
            stops.emplace_back(temp);
            tempsize-=1;
        }
    }    
    Transport::Bus bus(node->AsDict().at("name").AsString(),
                    std::move(stops),
                    node->AsDict().at("is_roundtrip").AsBool());
    catalogue_->AddBus(std::move(bus));
}

void JsonDataReader::SetDistancesForStopInCatalogue(const json::Node* node){
    std::string cur_stop_name = node->AsDict().at("name").AsString();
    Transport::Stop* curr_stop = const_cast<Transport::Stop*>(catalogue_->GetStopByName(cur_stop_name));
    for (auto [dest_stop_, distNode] : node->AsDict().at("road_distances").AsDict()) {
        //std::cout << "name: " << dest_stop_ << " dist: " << distNode.AsInt() << std::endl;
        const Transport::Stop* dest_stop = catalogue_->GetStopByName(dest_stop_);
        //curr_stop->distances_.emplace(dest_stop, distNode.AsInt());
        catalogue_->SetDistance(curr_stop, dest_stop, distNode.AsInt());
    }
}

void JsonDataReader::ParseStatRequests(const json::Node *node) {
    for (auto& node_ : node->AsArray()) {
        StatRequest* request = new StatRequest;
        request->id_ = node_.AsDict().at("id").AsInt();
        request->type_ = node_.AsDict().at("type").AsString();
        request->name_ = node_.AsDict().at("name").AsString();
        stat_requests_.push_back(request);
    }
    ProcessStatRequests();
}

std::tuple<double, int, int> JsonDataReader::ComputeRouteInfo(const Transport::Bus* bus) {
    double dist = 0.;
    int len = 0;
    std::set<std::string_view> unique_stops_;
    for (int i=1; i < bus->bus_stops_.size(); ++i) {
        auto from = bus->bus_stops_[i-1];
        auto to = bus->bus_stops_[i];
        len += catalogue_->GetDistance(from,to);
        dist += geo::ComputeDistance(from->coord_, to->coord_);
        unique_stops_.emplace(from->stop_name_);
        unique_stops_.emplace(to->stop_name_);
    } 
    return {dist,len,unique_stops_.size()};
}

json::Node JsonDataReader::MakeBusResp(int request_id, const Transport::Bus* bus) {
    json::Dict responce;
    Transport::BusInfo route;
    route.bus_no_ = bus->bus_name_;
    route.stops_count_ = bus->bus_stops_.size();
    route.route_distance_ = std::get<0>(ComputeRouteInfo(bus));
    route.route_length_ = std::get<1>(ComputeRouteInfo(bus));
    route.unique_stops_count_ = std::get<2>(ComputeRouteInfo(bus));
    route.curvature_ = route.route_length_*1.0 / route.route_distance_;
    

    responce.emplace("request_id", request_id);
    responce.emplace("curvature", route.curvature_);
    responce.emplace("route_length", route.route_length_);
    responce.emplace("stop_count", route.stops_count_);
    responce.emplace("unique_stop_count", route.unique_stops_count_);
    return responce;
}

json::Node JsonDataReader::MakeErrorResp(int request_id) {
    json::Dict responce;
    responce.emplace("request_id", request_id);
    responce.emplace("error_message"s, "not found"s);
    return responce;
}

json::Node JsonDataReader::MakeStopResp(int request_id, const Transport::Stop* stop) {
    json::Array buses;
    json::Dict responce;
    for (auto& bn : stop->buses_for_stop_) {
        //auto bus = catalogue_->GetBusByName(bn);
        std::string bns(bn);
        buses.push_back(bns);
    }
    responce.emplace("request_id", request_id);
    responce.emplace("buses", buses);
    return responce;
}

void JsonDataReader::ProcessStatRequests() {
    for (const StatRequest* req : stat_requests_) {
        if (req->type_ == "Stop") {
            const Transport::Stop* stop = catalogue_->GetStopByName(req->name_);
            if (stop) {
                responces_.emplace_back(MakeStopResp(req->id_, stop));
            } else {
                responces_.emplace_back(MakeErrorResp(req->id_));
            }

        }
        if (req->type_ == "Bus") {
            std::string bus_no_ = req->name_;
            const Transport::Bus* bus = catalogue_->GetBusByName(bus_no_);
            if (!bus) {responces_.emplace_back(MakeErrorResp(req->id_));}
            else {
                responces_.emplace_back(MakeBusResp(req->id_, bus));
            }
        }
    }
    json::Document r_doc(responces_);
    out_doc_ = std::move(r_doc);
    PrintResponces(std::cout);
}

json::Node JsonDataReader::GetStats() const {
    return responces_;
}

void JsonDataReader::PrintResponces(std::ostream& output) const{
    json::Print(out_doc_,output);
}
