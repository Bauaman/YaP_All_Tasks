#include "common.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <vector>
#include <iostream>
#include <cmath>

const int LETTERS = 26;
const int MAX_POSITION_LENGTH = 17;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

// Реализуйте методы:
bool Position::operator==(const Position rhs) const {
    if (!rhs.IsValid()) {
        return false;
    }
    return (row == rhs.row && col == rhs.col);
}

bool Position::operator<(const Position rhs) const {
    if (col < rhs.col) {
        return true;
    } else if (col == rhs.col) {
        return (row < rhs.row);
    } else {
        return false;
    }
}

bool Position::IsValid() const {
    if (row < 0 || col < 0 || row >= MAX_ROWS || col >= MAX_COLS) {
        return false;
    }
    return true;
}

std::string Position::ToString() const {
    if (!Position::IsValid()) {
        return {};
    }
    
    std::string result;
    int col_ = col;
    while (col_ >= 0) {
        char symb = ((col_ % LETTERS) + 'A');
        result.insert(result.begin(), symb);
        col_ = col_/LETTERS -1;
    }
    result += std::to_string(row + 1);
    return result;

}

Position Position::FromString(std::string_view str) {
    Position result;
    auto index = std::find_if(str.begin(), str.end(), isdigit);
    std::string col_(str.begin(), index);
    std::string row_(index, str.end());

    if (col_.size() == 0 || find_if(col_.begin(), col_.end(), 
                                    [](const char c){return !isupper(c);}) != col_.end()) {
        return NONE;
    }
    if (row_.size() == 0 || find_if(row_.begin(), row_.end(), 
                                    [](const char c){return !isdigit(c);}) != row_.end()) {
        return NONE;
    }
    for (char symb : col_) {
        result.col *= LETTERS;
        result.col += symb - 'A' + 1;
    }
    for (char digit : row_) {
        result.row *= 10;
        result.row += digit - '0';
    }
    result.row -=1;
    result.col -=1;

    

    return result;
}