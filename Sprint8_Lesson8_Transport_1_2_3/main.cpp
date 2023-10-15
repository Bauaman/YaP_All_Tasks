#include <iostream>
#include <string>
#include <vector>

#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace transport;
using namespace readers;

int main() {
    TransportCatalogue catalogue;
    //using namespace std::string_literals;
    int base_request_count;
    cin >> base_request_count >> ws;
    /*vector<string> commands = {"Stop Tolstopaltsevo: 55.611087, 37.208290"s,
                                "Stop Marushkino: 55.595884, 37.209755"s,
                                "Bus 256: Biryulyovo Zapadnoye > Biryusinka > Universam > Biryulyovo Tovarnaya > Biryulyovo Passazhirskaya > Biryulyovo Zapadnoye"s,
                                "Bus 750: Tolstopaltsevo - Marushkino - Rasskazovka"s,
                                "Stop Rasskazovka: 55.632761, 37.333324"s,
                                "Stop Biryulyovo Zapadnoye: 55.574371, 37.651700"s,
                                "Stop Biryusinka: 55.581065, 37.648390"s,
                                "Stop Universam: 55.587655, 37.645687"s,
                                "Stop Biryulyovo Tovarnaya: 55.592028, 37.653656"s,
                                "Stop Biryulyovo Passazhirskaya: 55.580999, 37.659164"s,
                                "Bus 828: Biryulyovo Zapadnoye > Universam > Rossoshanskaya ulitsa > Biryulyovo Zapadnoye"s,
                                "Stop Rossoshanskaya ulitsa: 55.595579, 37.605757"s,
                                "Stop Prazhskaya: 55.611678, 37.603831"s};
    */
    {
        InputReader reader;
        for (int i = 0; i < base_request_count /*commands.size()*/; ++i) {
            string line;// = commands[i];
            getline(cin, line);
            reader.ParseLine(line);
        }
        reader.ApplyCommands(catalogue);
    }

    int stat_request_count;
    cin >> stat_request_count >> ws;
    //vector<string> req = {"Bus 256"s, "Bus 750"s, "Bus 751"s, "Stop Samara"s, "Stop Prazhskaya"s, "Stop Biryulyovo Zapadnoye"s};
    for (int i = 0; i < stat_request_count /*req.size()*/; ++i) {
        string line; //= req[i];
        getline(cin, line);
        ParseAndPrintStat(catalogue, line, cout);
    }
}