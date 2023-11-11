#include <iostream>
#include <sstream>

#include "transport_catalogue.h"
#include "request_handler.h"

int main() {
    Transport::TransportCatalogue catalogue;
    auto input = Inputer::GetInputer();
    auto request_handler = RequestHandler::GetHandler();
    request_handler->LinkTransportCatalogue(catalogue);
    request_handler->ProcessInputRequests(input);
    request_handler->PrintCatalogueStats();
    //json::Document doc = Readers::LoadJson(str);
    //PrintType(doc);

}