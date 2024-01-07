#include <algorithm>
#include <cassert>
#include <optional>
#include <string_view>
#include <utility>
#include <vector>

using namespace std;

// напишите функцию ComputeStatistics, принимающую 5 параметров:
// два итератора, выходное значение для суммы, суммы квадратов и максимального элемента

template <typename iter, typename Tsum, typename TsqSum, typename Tmax>
void ComputeStatistics(iter it_begin, iter it_end, Tsum& sum, TsqSum& sq_sum, Tmax& max) {

    using Elem = std::decay_t<decltype(*it_begin)>;

    constexpr bool need_sum = !is_same_v<Tsum, const nullopt_t>;
    constexpr bool need_sq_sum = !is_same_v<TsqSum, const nullopt_t>;
    constexpr bool need_max = !is_same_v<Tmax, const nullopt_t>;
    
    auto local_sum = Elem{};
    if constexpr (need_sum) {
        local_sum = *it_begin;
    }    
    auto local_sq_sum = Elem{};
    if constexpr (need_sq_sum) {
        local_sq_sum = *it_begin * *it_begin;
    }
    auto local_max = Elem{};
    if constexpr (need_max) {
        local_max = *it_begin;
    } 

    while (it_begin != it_end)
    {
        ++it_begin;
        if constexpr (need_sum) {
            local_sum += *it_begin;
        }
        if constexpr (need_sq_sum) {
            local_sq_sum += *it_begin * *it_begin;
        }
        if constexpr (need_max) {
            if (local_max < *it_begin) {
                local_max = *it_begin;
            }
        }
    }
    

    if constexpr (need_sum) {
        sum = local_sum;        
    }
    if constexpr (need_sq_sum) {
        sq_sum = local_sq_sum;
    }
    if constexpr (need_max) {
        max = local_max;
    }

}

struct OnlySum {
    int value;
};

OnlySum operator+(OnlySum l, OnlySum r) {
    return {l.value + r.value};
}
OnlySum& operator+=(OnlySum& l, OnlySum r) {
    return l = l + r;
}

int main() {
    vector input = {1, 2, 3, 4, 5, 6};
    int sq_sum;
    std::optional<int> max;

    // Переданы выходные параметры разных типов - std::nullopt_t, int и std::optional<int>
    ComputeStatistics(input.begin(), input.end(), nullopt, sq_sum, max);

    assert(sq_sum == 91 && max && *max == 6);

    vector<OnlySum> only_sum_vector = {{100}, {-100}, {20}};
    OnlySum sum;

    // Поданы значения поддерживающие только суммирование, но запрошена только сумма
    ComputeStatistics(only_sum_vector.begin(), only_sum_vector.end(), sum, nullopt, nullopt);

    assert(sum.value == 20);
}