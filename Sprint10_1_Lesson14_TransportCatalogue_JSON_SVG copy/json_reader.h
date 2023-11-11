#pragma once
#include <deque>
#include <iostream>
#include <memory>
#include <sstream>
#include <unordered_map>

#include "domain.h"
#include "geo.h"
#include "json.h"
#include "request_handler.h"
#include "transport_catalogue.h"

struct StatRequest {
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

private:
    void ParseBaseRequests(const json::Node* node);
    void AddStopToCatalogue(const json::Node* node);
    void AddBusToCatalogue(const json::Node* node);
    void SetDistancesForStopInCatalogue(const json::Node* node);
    
    Transport::TransportCatalogue* catalogue_;

};

void PrintType(json::Document doc);
