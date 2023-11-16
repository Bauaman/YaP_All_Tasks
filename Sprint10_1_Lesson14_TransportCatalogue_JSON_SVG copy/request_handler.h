#pragma once
#include <iostream>
#include <stack>
#include <unordered_map>
#include <memory>

#include "transport_catalogue.h"



/*
 * Здесь можно было бы разместить код обработчика запросов к базе, содержащего логику, которую не
 * хотелось бы помещать ни в transport_catalogue, ни в json reader.
 *
 * В качестве источника для идей предлагаем взглянуть на нашу версию обработчика запросов.
 * Вы можете реализовать обработку запросов способом, который удобнее вам.
 *
 * Если вы затрудняетесь выбрать, что можно было бы поместить в этот файл,
 * можете оставить его пустым.
 */

// Класс RequestHandler играет роль Фасада, упрощающего взаимодействие JSON reader-а
// с другими подсистемами приложения.
// См. паттерн проектирования Фасад: https://ru.wikipedia.org/wiki/Фасад_(шаблон_проектирования)

class Inputer {
public:
    static Inputer* GetInputer();
    virtual std::istream& GetStream() = 0;
};

class ConsoleInputer final : public Inputer {
public:
    std::istream& GetStream() override;
};

class RequestHandler {
public:
    // MapRenderer понадобится в следующей части итогового проекта
    RequestHandler() = default;

    virtual ~RequestHandler() = default;

    RequestHandler(Transport::TransportCatalogue& tc, Inputer* input) : //, const renderer::MapRenderer& renderer);
        transport_catalogue_(&tc),
        input_(input)
    {
        ProcessInputRequests(input_);
    }
    
    // Возвращает информацию о маршруте (запрос Bus)
    //std::optional<BusStat> GetBusStat(const std::string_view& bus_name) const;

    // Возвращает маршруты, проходящие через
    //const std::unordered_set<BusPtr>* GetBusesByStop(const std::string_view& stop_name) const;

    // Этот метод будет нужен в следующей части итогового проекта
    //svg::Document RenderMap() const;

    void ProcessInputRequests(Inputer* input);

    //static RequestHandler *GetHandler();

    void PrintCatalogueStats();

    void SetInputer(Inputer* input) {
        input_ = input;
    }

    void LinkTransportCatalogue(Transport::TransportCatalogue& tc) {
        transport_catalogue_ = &tc;
    }
    protected:
        
    // RequestHandler использует агрегацию объектов "Транспортный Справочник" и "Визуализатор Карты"
        Transport::TransportCatalogue* transport_catalogue_ = nullptr;
        Inputer* input_;
    //const renderer::MapRenderer& renderer_;
};
