#pragma once

#include "cell.h"
#include "common.h"

#include <functional>

class Hasher {
public:
    size_t operator()(const Position pos) const {
        return std::hash<std::string>()(pos.ToString());
    }
};

class Comp {
public:
    bool operator()(const Position& lhs, const Position& rhs) const {
        return lhs == rhs;
    }
};

class Sheet : public SheetInterface {
public:
    ~Sheet();

    void SetCell(Position pos, std::string text) override;

    const CellInterface* GetCell(Position pos) const override;
    CellInterface* GetCell(Position pos) override;

    void ClearCell(Position pos) override;

    Size GetPrintableSize() const override;

    void PrintValues(std::ostream& output) const override;
    void PrintTexts(std::ostream& output) const override;

	// Можете дополнить ваш класс нужными полями и методами

private:
	// Можете дополнить ваш класс нужными полями и методами
    std::unordered_map<Position, Cell, Hasher, Comp> cells_;
};