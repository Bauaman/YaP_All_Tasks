#pragma once
#include <iostream>
#include <string>
#include <string_view>
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
                int base_request_count = 1;
                //input >> base_request_count >> std::ws;

                for (int i = 0; i < base_request_count; ++i) {
                    std::string line = "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye";
                    //std::getline(input, line);
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
            CommandDescription ParseCommandDescription(std::string_view line);
            CommandDescription ParseRequestCommand(std::string_view line);
        } //namespace detail
    } //namespace readers
} //namespace transport