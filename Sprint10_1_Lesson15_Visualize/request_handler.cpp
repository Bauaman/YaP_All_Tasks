#include "request_handler.h"
#include "json_reader.h"

void RequestHandler::LinkTransportCatalogue(TransportCatalogue& tc) {
    catalogue_ = &tc;
}

void RequestHandler::GetHandler() {
    handler_ = new JsonDataReader(catalogue_);
}

void RequestHandler::ProcessRequests(std::istream& input) {
    handler_->ProcessRequests(input);
}

void RequestHandler::ProcessStatRequests() {
    if (!renderer_) {
        GetMapRenderer();
    }
    handler_->ProcessStatRequests(renderer_);
}

void RequestHandler::PrintStatsJson(std::ostream& out) {
    handler_->PrintStatsAsJson(out);
}

void RequestHandler::GetMapRenderer() {
    renderer_ = new MapRenderer(handler_->ProcessRenderSettings());
}

void RequestHandler::RenderMap() {
    if (!renderer_) {
        GetMapRenderer();
    }
    svg::Document doc;
    std::map<std::string_view, const Bus*> buses = catalogue_->GetSortedBuses();
    std::map<std::string_view, const Stop*> stops = catalogue_->GetSortedStops();
    doc = renderer_->GetSVGDoc(buses, stops);
    doc.Render(std::cout);   
}

