#include <vector>
#include <string_view>
#include <string>
#include <iostream>

std::string GenerateText() {
    const int SIZE = 10000000;
    std::string text(SIZE, 'a');
    for (int i = 100; i < SIZE; i += 100) {
        text[i] = ' ';
    }
    return text;
}

std::vector<std::string_view> SplitIntoWordsView(const std::string& str) {
    std::vector<std::string_view> result;
    // 1
    auto pos = str.find_first_not_of(" ");
    // 2
    const auto pos_end = str.npos;
    // 3
    while (pos != pos_end) {
        // 4
        auto space = str.find(' ', pos);
        // 5
        result.push_back(space == pos_end ? str.substr(pos) : str.substr(pos, space - pos));
        // 6
        pos = str.find_first_not_of(" ", space);
    }

    return result;
}

int main() {
        const std::string text = "a b"; //GenerateText();

        const auto words = SplitIntoWordsView(text);
        // выведем первое слово
        std::cout << words[0] << "\n";
}