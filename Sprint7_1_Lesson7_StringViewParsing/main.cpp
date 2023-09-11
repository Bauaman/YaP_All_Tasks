#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string_view>
#include <vector>


using namespace std;

std::vector<std::string_view> SplitIntoWordsView(std::string_view str) {
    std::vector<std::string_view> result;
    str.remove_prefix(std::min(str.size(), str.find_first_not_of(' ')));
 
    while (!str.empty()) {
        
        auto space = str.find(' ');
        result.push_back(space == str.npos ? str.substr(0, str.size()) : str.substr(0, space));
        
        str.remove_prefix(std::min(str.size(), str.find_first_not_of(' ', space)));
    }
    return result;
}

int main() {
    assert((SplitIntoWordsView("") == vector<string_view>{}));
    assert((SplitIntoWordsView("     ") == vector<string_view>{}));
    assert((SplitIntoWordsView("aaaaaaa") == vector{"aaaaaaa"sv}));
    assert((SplitIntoWordsView("a") == vector{"a"sv}));
    assert((SplitIntoWordsView("a b c") == vector{"a"sv, "b"sv, "c"sv}));
    assert((SplitIntoWordsView("a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    assert((SplitIntoWordsView("  a    bbb   cc   ") == vector{"a"sv, "bbb"sv, "cc"sv}));
    cout << "All OK" << endl;
}