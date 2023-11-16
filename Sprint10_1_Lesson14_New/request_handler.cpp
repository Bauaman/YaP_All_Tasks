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

void RequestHandler::PrintStatsJson(std::ostream& out) {
    handler_->PrintStatsAsJson(out);
}
