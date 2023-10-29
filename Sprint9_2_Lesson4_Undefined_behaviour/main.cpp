#include <iostream>
#include <string>
#include <string_view>

using namespace std;


// clang-format off

//const int NUM_PLANETS = 9;
const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

// clang-format on

bool IsPlanet(string_view name) {
    for (string_view planet : PLANETS) {
        if (planet == name) {
            return true;
        }
    }
    return false;
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv)
        << "a planet"sv << endl;
}

istream& ReadInput (istream& in, string& input_planet) {
    getline(in, input_planet);
    return in;
}

int main() {
    string name;
    ReadInput(cin, name);
    Test(name);

}