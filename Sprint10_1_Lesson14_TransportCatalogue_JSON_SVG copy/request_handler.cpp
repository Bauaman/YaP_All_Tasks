#include "request_handler.h"
#include "json_reader.h"

/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

Inputer* Inputer::GetInputer() {
    Inputer* p = nullptr;
    p = new ConsoleInputer();
    return p;
}

std::istream& ConsoleInputer::GetStream() {
    return std::cin;
}

/*
RequestHandler* RequestHandler::GetHandler() {
    RequestHandler* p = nullptr;
    p = new JsonDataReader();
    return p;
}
*/

void RequestHandler::ProcessInputRequests(Inputer* input) {
    JsonDataReader* p_ = new JsonDataReader(transport_catalogue_);
    p_->ProcessInputRequests(input);
    
}

void RequestHandler::PrintCatalogueStats() {
    std::cout << "buses: " << transport_catalogue_->GetBusQty() << std::endl;
    std::cout << "stops: " << transport_catalogue_->GetStopQty() << std::endl;
}