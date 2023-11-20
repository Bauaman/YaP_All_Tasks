#include "request_handler.h"
#include "transport_catalogue.h"

int main() {
    TransportCatalogue transport_catalogue;
    RequestHandler handler(transport_catalogue);
    handler.ProcessRequests(std::cin);
    handler.ProcessStatRequests();
    handler.PrintStatsJson(std::cout);
    //handler.RenderMap();
    
}