#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <numeric>
#include <vector>

// не меняйте файлы json.h и json.cpp
#include "json.h"

using namespace std;

struct Spending {
    string category;
    int amount;
};

int CalculateTotalSpendings(const vector<Spending>& spendings) {
    return accumulate(
        spendings.begin(), spendings.end(), 0, [](int current, const Spending& spending){
            return current + spending.amount;
        });
}

string FindMostExpensiveCategory(const vector<Spending>& spendings) {
    assert(!spendings.empty());
    auto compare_by_amount = [](const Spending& lhs, const Spending& rhs) {
        return lhs.amount < rhs.amount;
    };
    return max_element(begin(spendings), end(spendings), compare_by_amount)->category;
}

vector<Spending> LoadFromJson(istream& input) {
    // место для вашей реализации
    // пример корректного JSON-документа в условии
    vector<Spending> result;
    Document docs = Load(input);
    for (auto& doc : docs.GetRoot().AsArray()) {
        string category = doc.AsMap().at("category").AsString();
        int amt = doc.AsMap().at("amount").AsInt();
        result.push_back({category,amt});
    }
    return result;
}

int main() {
    // не меняйте main
    ifstream inp;
    inp.open("test.json");
    const vector<Spending> spendings = LoadFromJson(inp);
    cout << "Total "sv << CalculateTotalSpendings(spendings) << '\n';
    cout << "Most expensive is "sv << FindMostExpensiveCategory(spendings) << '\n';
}