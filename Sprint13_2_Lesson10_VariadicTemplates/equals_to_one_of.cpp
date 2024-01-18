#include <cassert>
#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;
/* Напишите вашу реализацию EqualsToOneOf здесь*/
template <typename T0, typename... Tn>
bool EqualsToOneOf(const T0& v0, const Tn&... vn) {
    if constexpr (sizeof...(vn) == 0) {
        return false;
    }
    bool res = false;
    (..., (res || (v0 == vn ? (res = true) : (res = false))));
    return res;
}

int main() {
    assert(EqualsToOneOf("hello"sv, "hi"s, "hello"s));
    assert(!EqualsToOneOf(1, 10, 2, 3, 6));
    assert(!EqualsToOneOf(8));
}