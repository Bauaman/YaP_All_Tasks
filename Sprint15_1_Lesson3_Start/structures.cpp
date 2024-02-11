#include "common.h"

#include <algorithm>
#include <cctype>
#include <sstream>

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
    if (row < 0 || col < 0 || row > MAX_ROWS || col > MAX_COLS) {
        return false;
    }
    return true;
}

std::string Position::ToString() const {
    if (!Position::IsValid()) {
        return {};
    }   
}

Position Position::FromString(std::string_view str) {
    Position result;
    auto index = std::find_if(str.begin(), str.end(), isdigit);
    std::string col_(str.begin(), index);
    for (size_t i = 0; i<col_.size(); ++i) {
        result.col = (col_[i]-'A');
    }
}