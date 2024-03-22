#include "sheet.h"

#include "cell.h"
#include "common.h"

#include <algorithm>
#include <functional>
#include <iostream>
#include <optional>

using namespace std::literals;

Sheet::~Sheet() {}

void Sheet::SetCell(Position pos, std::string text) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    //cells_[pos].Set(text);
    //Убедимся, что у нас достаточно строк и столбцов для позиции
    if (pos.row >= static_cast<int>(cells_.size())) {
        cells_.resize(pos.row + 1);
    }
    if (pos.col >= static_cast<int>(cells_[pos.row].size())) {
        cells_[pos.row].resize(pos.col + 1);
    }
    cells_[pos.row][pos.col] = std::make_unique<Cell>();
    cells_[pos.row][pos.col] -> Set(text);
}

const CellInterface* Sheet::GetCell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    /*
    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    const Cell* cell = &cells_.at(pos);
    if (cell->GetText().size() == 0) {
        return nullptr;
    }
    return cell;
    */

    if (pos.row < static_cast<int>(cells_.size()) && pos.col < static_cast<int>(cells_[pos.row].size())) {
        return cells_[pos.row][pos.col].get();
    }
    
    return nullptr;
}

CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    /*
    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    Cell* cell = &cells_.at(pos);
    if (cell->GetText().size() == 0) {
        return nullptr;
    }
    return cell;
    */
    if (pos.row < static_cast<int>(cells_.size()) && pos.col < static_cast<int>(cells_[pos.row].size())) {
        return cells_[pos.row][pos.col].get();
    }
    
    return nullptr;
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    /*
    cells_[pos].Clear();
    */
    if (pos.row < static_cast<int>(cells_.size()) && pos.col < static_cast<int>(cells_[pos.row].size())) {
        if (cells_[pos.row][pos.col]) {
            cells_.at(pos.row).at(pos.col)->Clear();
        }
    }
}

Size Sheet::GetPrintableSize() const {
    /*
    if (cells_.begin() == cells_.end()) {
        return {0,0};
    }
    Size result{0, 0};
    for (auto iter = cells_.begin(); iter != cells_.end(); ++iter) {
        if (iter->second.GetText().size() == 0) {
            continue;
        }
        const int column = iter->first.col;
        const int row = iter->first.row;
        if (result.cols <= column) {
            result.cols = column + 1;
        }
        if (result.rows <= row) {
            result.rows = row + 1;
        }
    }
    return result;
*/

    if (cells_.begin() == cells_.end()) {
        return {0,0};
    }
/*
    if (cells_.empty() || cells_[0].empty()) {
        return {0, 0};
    }
*/

    Size result{0, 0};
    for (int row = 0; row < static_cast<int>(cells_.size()); ++row) {
        for (int col = static_cast<int>(cells_[row].size() - 1); col >= 0; --col) {
            if (cells_[row][col]) {
                if (cells_[row][col]->GetText().empty()) {
                    continue;
                } else {
                    result.rows = std::max(result.rows, row + 1);
                    result.cols = std::max(result.cols, col + 1);
                    break;
                }
            }
        }
    }
    return result;
}
/*
    for (size_t row = 0; row < cells_.size(); ++row) {
        for (size_t col = 0; col < cells_[row].size(); ++col) {
            if (cells_[row][col] && !cells_[row][col]->GetText().empty()) {
                if (result.cols <= static_cast<int>(col)) {
                    result.cols = col + 1;
                }
                if (result.rows <= static_cast<int>(row)) {
                    result.rows = row + 1;
                }
            }
        }
    }

    return result;
}
*/

void Sheet::PrintCells(std::ostream& output, const std::function<void(const CellInterface&)>& printCell) const {
    Size size = GetPrintableSize();
    for (int row = 0; row < size.rows; ++row) {
        for (int col = 0; col < size.cols; ++col) {
            if (col) {
                output << "\t";
            }
            if (col < static_cast<int>(cells_[row].size())) {
                const CellInterface* cell = cells_[row][col].get();
                if (cell) {
                    printCell(*cell);
                }
            }
            
            /*
            auto it = cells_.find({row,col});
            const CellInterface* cell = &it->second;
            if (it != cells_.end()) {
                printCell(*cell);
            }
        }*/

        }
        output << "\n";
    }
}

std::ostream& operator<<(std::ostream& out, const CellInterface::Value& val) {
    std::visit([&out](const auto& v) {
        out << v;
    }, val);
    return out;
}

void Sheet::PrintValues(std::ostream& output) const {
    PrintCells(output, [&output](const CellInterface& cell) {
        output << cell.GetValue();
    });
}

void Sheet::PrintTexts(std::ostream& output) const {
    PrintCells(output, [&output](const CellInterface& cell) {
        output << cell.GetText();
    });
}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}