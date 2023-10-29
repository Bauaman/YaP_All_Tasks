#include "input_reader.h"

#include <algorithm>
#include <cassert>
#include <iterator>
#include <optional>
#include <string>
#include <string_view>

/**
 * Парсит строку вида "10.123,  -30.1837" и возвращает пару координат (широта, долгота)
 */

namespace transport {
    namespace readers {
        namespace detail {

            geo::Coordinates ParseCoordinates(std::string_view str) {
                static const double nan = std::nan("");

                auto not_space = str.find_first_not_of(' ');
                auto comma = str.find(',');

                if (comma == str.npos) {
                    return {nan, nan};
                }

                auto not_space2 = str.find_first_not_of(' ', comma + 1);

                double lat = std::stod(std::string(str.substr(not_space, comma - not_space)));
                double lng = std::stod(std::string(str.substr(not_space2)));

                return {lat, lng};
            }

            int StringToInt(std::string& str) {
                int res = 0;
                for (int i=0; i<str.size(); i++) {
                    res += (str[i] - '0')*std::pow(10, str.size()-1-i);
                }
                return res;
            }

            std::string AsString(std::string_view strv) {
                return {strv.begin(), strv.end()};
            }

        /**
         * Удаляет пробелы в начале и конце строки
         */
            std::string_view Trim(std::string_view string) {
                const auto start = string.find_first_not_of(' ');
                if (start == string.npos) {
                    return {};
                }
                return string.substr(start, string.find_last_not_of(' ') + 1 - start);
            }

            /**
             * Разбивает строку string на n строк, с помощью указанного символа-разделителя delim
             */
            std::vector<std::string_view> Split(std::string_view string, char delim) {
                std::vector<std::string_view> result;

                size_t pos = 0;
                while ((pos = string.find_first_not_of(' ', pos)) < string.length()) {
                    auto delim_pos = string.find(delim, pos);
                    if (delim_pos == string.npos) {
                        delim_pos = string.size();
                    }
                    if (auto substr = Trim(string.substr(pos, delim_pos - pos)); !substr.empty()) {
                        result.push_back(substr);
                    }
                    pos = delim_pos + 1;
                }

                return result;
            }

            std::optional<std::string_view> ParseDistancesFromDescription(std::string_view str) {
                size_t pos = 0;
                int num = 0;
                while ((pos = str.find_first_of(',',pos+1)) < str.length()) {
                        if (++num == 2) {
                            return (Trim(str.substr(pos+1)));
                        }
                }
                return std::nullopt;
            }

            /**
             * Парсит маршрут.
             * Для кольцевого маршрута (A>B>C>A) возвращает массив названий остановок [A,B,C,A]
             * Для некольцевого маршрута (A-B-C-D) возвращает массив названий остановок [A,B,C,D,C,B,A]
             */
            std::vector<std::string_view> ParseRoute(std::string_view route) {
                if (route.find('>') != route.npos) {
                    return Split(route, '>');
                }

                auto stops = Split(route, '-');
                std::vector<std::string_view> results(stops.begin(), stops.end());
                results.insert(results.end(), std::next(stops.rbegin()), stops.rend());

                return results;
            }


            CommandDescription ParseCommandDescription(std::string_view line) {
                auto colon_pos = line.find(':');
                if (colon_pos == line.npos) {
                    return {};
                }

                auto space_pos = line.find(' ');
                if (space_pos >= colon_pos) {
                    return {};
                }

                auto not_space = line.find_first_not_of(' ', space_pos);
                if (not_space >= colon_pos) {
                    return {};
                }

                return {std::string(line.substr(0, space_pos)),
                        std::string(line.substr(not_space, colon_pos - not_space)),
                        std::string(line.substr(colon_pos + 1))};
            }

            CommandDescription ParseRequestCommand(std::string_view line) {
                auto space_pos = line.find(' ');
                auto not_space = line.find_first_not_of(' ', space_pos);
                return {std::string(line.substr(0, space_pos)),
                        std::string(line.substr(not_space)),
                        {}};
            }
            
        } // namespace detail
            
        void InputReader::ParseLine(std::string_view line) {
            auto command_description = detail::ParseCommandDescription(line);
            if (command_description) {
                commands_.push_back(std::move(command_description));
            }
        }

        void InputReader::ApplyCommands([[maybe_unused]] transport::TransportCatalogue& catalogue) const {
            // Реализуйте метод самостоятельно
            std::vector<CommandDescription> bus_requests;
            std::unordered_map<const transport::Stop*, std::string_view> distances_for_stop;
            for (const CommandDescription& cd : commands_) {
                if (cd.command == "Stop") {
                    transport::Stop* curr_stop = new transport::Stop(cd.id, detail::ParseCoordinates(cd.description));
                    catalogue.AddStop(std::move(*curr_stop));
                    auto routes = detail::ParseDistancesFromDescription(cd.description);
                    if (routes) {
                        const Stop* st = catalogue.GetStopByName(cd.id);
                        distances_for_stop[st] = routes.value();
                    }

                }
                if (cd.command == "Bus") {
                bus_requests.push_back(cd);
                }
            }

            for (auto [stop,str] : distances_for_stop) {
                std::vector<std::string_view> destinations_for_stop = detail::Split(str, ',');
                for (std::string_view dest : destinations_for_stop) {
                    size_t pos = dest.find_first_of('m');
                    std::string str_dest = detail::AsString(dest);
                    std::string meters_s = (str_dest.substr(0,pos));
                    int meters_i = detail::StringToInt(meters_s);                     
                    //std::string dist = detail::Trim(dest.substr(0, dest.find_first_of('m',0))).data();

                    std::string_view dest_name = detail::Trim(dest.substr(dest.find_first_of('o',0)+1));
                    const Stop* stop_dest =  catalogue.GetStopByName(detail::AsString(dest_name));
                    Key pair_stops {stop, stop_dest};
                    Key pair_stops_reverse {stop_dest, stop};
                    catalogue.FillDistBwStops(pair_stops, meters_i);
                    bool contains_reverse = catalogue.CheckIfContainsPair(pair_stops_reverse);
                    if (!contains_reverse) {
                        catalogue.FillDistBwStops(pair_stops_reverse, meters_i);
                    }
                }
            }
            
            for (const CommandDescription& cd : bus_requests) {
                transport::Bus* curr_bus = new transport::Bus(cd.id, detail::ParseRoute(cd.description));
                catalogue.AddBus(std::move(*curr_bus));
                catalogue.FillStopToRoute(*curr_bus);
                catalogue.AddRoute(*curr_bus);
            }
        }
    }//namespace readers
}//namespace transport