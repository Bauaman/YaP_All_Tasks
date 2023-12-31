#include <iostream>
#include <string>
#include <vector>

#include "input_reader.h"
#include "stat_reader.h"

using namespace std;
using namespace transport;
using namespace transport::readers;

int main() {
    TransportCatalogue catalogue;

    //int base_request_count;
    //cin >> base_request_count >> ws;

    {
        InputReader reader (cin, catalogue);
        /*
        for (int i = 0; i < base_request_count; ++i) {
            string line;
            getline(cin, line);
            reader.ParseLine(line);
        }
        reader.ApplyCommands(catalogue);*/
    }
    StatRequestAndPrint(cin, cout, catalogue);
    /*
    int stat_request_count;
    cin >> stat_request_count >> ws;

    for (int i = 0; i < stat_request_count; ++i) {
        string line;
        getline(cin, line);
        ParseAndPrintStat(catalogue, line, cout);
    }*/
}