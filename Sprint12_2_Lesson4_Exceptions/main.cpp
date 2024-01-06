#include "budget_manager.h"
#include "parser.h"

#include <iostream>
#include <string_view>

using namespace std::string_literals;
using namespace std::string_view_literals;

void ParseAndProcessQuery(BudgetManager& manager, std::string_view line) {
    std::string_view query_type = line.substr(0, line.find_first_of(' '));
    line.remove_prefix(query_type.size()+1);
    if (query_type == "Earn"sv) {
        ParserModify parser(line);
        manager.Earn(parser.GetDateFrom(), parser.GetDateTo(), parser.GetValue());
    }
    if (query_type == "PayTax"sv) {
        ParserModify parser(line);
        manager.PayTax(parser.GetDateFrom(), parser.GetDateTo(), parser.GetValue());
    }
    if (query_type == "ComputeIncome"sv) {
        ParserRead parser(line);
        std::cout << manager.ComputeIncome(parser.GetDateFrom(), parser.GetDateTo()) << std::endl;
    }
    if (query_type == "Spend"sv) {
        ParserModify parser(line);
        manager.Spend(parser.GetDateFrom(), parser.GetDateTo(), parser.GetValue());
    }
}

int ReadNumberOnLine(std::istream& input) {
    std::string line;
    std::getline(input, line);
    return std::stoi(line);
}

int main() {
    BudgetManager manager;

    const int query_count = ReadNumberOnLine(std::cin);

    for (int i = 0; i < query_count; ++i) {
        std::string line;
        std::getline(std::cin, line);
        ParseAndProcessQuery(manager, line);
    }
}
