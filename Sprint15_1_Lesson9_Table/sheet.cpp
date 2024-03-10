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
    cells_[pos].Set(text);
}

const CellInterface* Sheet::GetCell(Position pos) const {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    const Cell* cell = &cells_.at(pos);
    if (cell->GetText().size() == 0) {
        return nullptr;
    }
    return cell;
}

CellInterface* Sheet::GetCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    auto it = cells_.find(pos);
    if (it == cells_.end()) {
        return nullptr;
    }
    Cell* cell = &cells_.at(pos);
    if (cell->GetText().size() == 0) {
        return nullptr;
    }
    return cell;
}

void Sheet::ClearCell(Position pos) {
    if (!pos.IsValid()) {
        throw InvalidPositionException("Invalid position");
    }
    cells_[pos].Clear();
}

Size Sheet::GetPrintableSize() const {
    if (cells_.begin() == cells_.end()) {
        return {0,0};
    }

}

void Sheet::PrintValues(std::ostream& output) const {}
void Sheet::PrintTexts(std::ostream& output) const {}

std::unique_ptr<SheetInterface> CreateSheet() {
    return std::make_unique<Sheet>();
}