#pragma once
#include "date.h"
#include <iostream>
#include <vector>

struct DailyData {
    double date_income = 0;
};

struct DaysInterval {
    int begin_interval;
    int interval_depth;
};

class BudgetManager {
public:
    inline static const Date START_DATE{2000, 1, 1};
    inline static const Date END_DATE{2100, 1, 1};
    BudgetManager() :
        budget_(END_DATE.ComputeDistance(START_DATE, END_DATE), DailyData())
    {}

    double ComputeIncome(Date begin_compute_interval, Date end_compute_interval);
    void Earn(Date begin_earn_interval, Date end_earn_interval, double income_amount);
    void PayTax(Date begin_tax_interval, Date end_tax_interval);

private:
    std::vector<DailyData> budget_;
    int ComputeDistanceFromFirstDate(Date date);
    DaysInterval ComputeDaysInterval(Date begin, Date end);
};
