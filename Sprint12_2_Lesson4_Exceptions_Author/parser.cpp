#include "parser.h"

using namespace std;

namespace queries {
class ComputeIncome : public ComputeQuery {
public:
    using ComputeQuery::ComputeQuery;
    ReadResult Process(const BudgetManager& budget) const override {
        const int idx_from = budget.GetDayIndex(GetFrom());
        const int idx_to = budget.GetDayIndex(GetTo());

        double income = 0;

        for (int i = idx_from; i <= idx_to; ++i) {
            income += budget.GetDayState(i).ComputeIncome();
        }

        return {income};
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
        }
    };
};

class Alter : public ModifyQuery {
public:
    Alter(Date from, Date to, double amount)
        : ModifyQuery(from, to)
        , amount_(amount) {
    }

    void Process(BudgetManager& budget) const override {
        const int idx_from = budget.GetDayIndex(GetFrom());
        const int idx_to = budget.GetDayIndex(GetTo());

        double day_income = amount_ / (idx_to - idx_from + 1);

        for (int i = idx_from; i <= idx_to; ++i) {
            if (day_income > 0) {
                budget.GetDayState(i).income += day_income;
            } else {
                budget.GetDayState(i).spent += -day_income;
            }
        }
    }

    class FactoryIncome : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            double payload = std::stod(std::string(parts[2]));
            return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
        }
    };

    class FactorySpend : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            double payload = std::stod(std::string(parts[2]));
            return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), -payload);
        }
    };

private:
    double amount_;
};

class PayTax : public ModifyQuery {
public:
    PayTax(Date from, Date to, double rate)
        : ModifyQuery(from, to)
        , rate_(rate) {
    }

    void Process(BudgetManager& budget) const override {
        const int idx_from = budget.GetDayIndex(GetFrom());
        const int idx_to = budget.GetDayIndex(GetTo());

        for (int i = idx_from; i <= idx_to; ++i) {
            budget.GetDayState(i).income *= (1 - rate_);
        }
    }

    class Factory : public QueryFactory {
    public:
        std::unique_ptr<Query> Construct(std::string_view config) const override {
            auto parts = Split(config, ' ');
            int payload = std::stoi(std::string(parts[2]));
            return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload / 100.);
        }
    };

private:
    double rate_;
};

}  // namespace queries

const QueryFactory& QueryFactory::GetFactory(std::string_view id) {
    static queries::ComputeIncome::Factory compute_income;
    static queries::Alter::FactoryIncome earn;
    static queries::Alter::FactorySpend spend;
    static queries::PayTax::Factory pay_tax;
    static std::unordered_map<std::string_view, const QueryFactory&> factories
        = {{"ComputeIncome"sv, compute_income}, {"Earn"sv, earn}, {"Spend"sv, spend}, {"PayTax"sv, pay_tax}};

    return factories.at(id);
}