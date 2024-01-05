// разработайте менеджер личного бюджета самостоятельно
#include "budget_manager.h"

int BudgetManager::ComputeDistanceFromFirstDate(Date date) {
    return (date.ComputeDistance(START_DATE, date));
}

DaysInterval BudgetManager::ComputeDaysInterval(Date begin, Date end) {
    DaysInterval result;
    result.begin_interval = ComputeDistanceFromFirstDate(begin);
    result.interval_depth = ComputeDistanceFromFirstDate(end) - result.begin_interval +1;
    return result;
}

void BudgetManager::Earn(Date begin_earn_interval, Date end_earn_interval, double income_amount) {
    DaysInterval interval = ComputeDaysInterval(begin_earn_interval, end_earn_interval);
    for (int i = interval.begin_interval; i < interval.begin_interval + interval.interval_depth; ++i) {
        budget_[i].date_income += income_amount / interval.interval_depth;
    }
}

void BudgetManager::PayTax(Date begin_tax_interval, Date end_tax_interval) {
    DaysInterval interval = ComputeDaysInterval(begin_tax_interval, end_tax_interval);
    for (int i = interval.begin_interval; i < interval.begin_interval + interval.interval_depth; ++i) {
        budget_[i].date_income *= 0.87;
    }
}

double BudgetManager::ComputeIncome(Date begin_compute_interval, Date end_compute_interval) {
    DaysInterval interval = ComputeDaysInterval(begin_compute_interval, end_compute_interval);
    double income = 0;
    for (int i = interval.begin_interval; i < interval.begin_interval + interval.interval_depth; ++i) {
        income += budget_[i].date_income;
    }
    return income;
}