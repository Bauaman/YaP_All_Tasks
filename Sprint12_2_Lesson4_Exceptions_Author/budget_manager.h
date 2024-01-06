#pragma once
#include "date.h"

#include <vector>

struct DayState {
    double ComputeIncome() const {
        return income - spent;
    }

    double income = 0;
    double spent = 0;
};

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};

    static int GetDayIndex(Date day) {
        return Date::ComputeDistance(START_DATE, day);
    }

    DayState& GetDayState(int index) {
        return days_[index];
    }

    const DayState& GetDayState(int index) const {
        return days_[index];
    }

private:
    std::vector<DayState> days_ = std::vector<DayState>(GetDayIndex(END_DATE));
};