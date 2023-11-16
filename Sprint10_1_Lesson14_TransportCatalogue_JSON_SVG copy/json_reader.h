#pragma once
#include <algorithm>
#include <deque>
#include <iostream>
#include <memory>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <variant>
#include <vector>
#include <set>

#include "domain.h"
#include "geo.h"
#include "json.h"
#include "request_handler.h"
#include "transport_catalogue.h"

struct StatRequest {
    StatRequest() = default;
    StatRequest(int id, std::string type, std::string name) :
        id_(id),
        type_(type),
        name_(name)
    {}

    int id_;
    std::string type_;
    std::string name_;
};


class JsonDataReader {
public:
    JsonDataReader() = default;

    JsonDataReader(Transport::TransportCatalogue* catalogue) :
        catalogue_(catalogue)
    {}

    void ProcessInputRequests(Inputer* input);
    void PrintResponces(std::ostream& output) const;
    json::Node GetStats() const;



private:
    void ParseBaseRequests(const json::Node* node);
    void AddStopToCatalogue(const json::Node* node);
    void AddBusToCatalogue(const json::Node* node);
    void SetDistancesForStopInCatalogue(const json::Node* node);
    void ParseStatRequests(const json::Node *node);
    void ProcessStatRequests();
    json::Node MakeBusResp(int request_id, const Transport::Bus* bus);
    json::Node MakeErrorResp(int request_id);
    json::Node MakeStopResp(int request_id, const Transport::Stop* stop);
    std::tuple<double, int, int> ComputeRouteInfo(const Transport::Bus* bus);

    Transport::TransportCatalogue* catalogue_;
    std::vector<const StatRequest*> stat_requests_;
    json::Array responces_;
    json::Document out_doc_;
};

