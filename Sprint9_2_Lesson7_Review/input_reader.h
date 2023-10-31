#pragma once
#include <iostream>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "geo.h"
#include "transport_catalogue.h"

namespace transport {
    namespace readers {

        struct CommandDescription {
            // Определяет, задана ли команда (поле command непустое)

            explicit operator bool() const {
                return !command.empty();
            }

            bool operator!() const {
                return !operator bool();
            }

            std::string command;      // Название команды
            std::string id;           // id маршрута или остановки
            std::string description;  // Параметры команды

        };

        class InputReader {
        public:

            InputReader(std::istream& input, TransportCatalogue& catalogue) {
                
                int base_request_count;
                input >> base_request_count >> std::ws;

                for (int i = 0; i < base_request_count; ++i) {
                    std::string line;
                    std::getline(input, line);
                    ParseLine(line);
                }
                ApplyCommands(catalogue);
            }
            /**
             * Парсит строку в структуру CommandDescription и сохраняет результат в commands_
             */
            void ParseLine(std::string_view line);

            /**
             * Наполняет данными транспортный справочник, используя команды из commands_
             */
            void ApplyCommands(transport::TransportCatalogue& catalogue) const;

        private:
            std::vector<CommandDescription> commands_;
        };
        namespace detail {
            std::string AsString(std::string_view strv);
            CommandDescription ParseCommandDescription(std::string_view line);
            CommandDescription ParseRequestCommand(std::string_view line);
            std::tuple<std::string_view, int> ParseDestination(std::string_view dest);
        } //namespace detail
    } //namespace readers
} //namespace transport