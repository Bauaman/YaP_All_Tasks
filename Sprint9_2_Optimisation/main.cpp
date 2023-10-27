#include <limits>
#include <cstdint>
#include <iostream>
#include <optional>


using namespace std;

template <typename T>
optional<T> SafeAdd(T a, T b) {
    if (a>0 && b>0) {
        if (a > std::numeric_limits<T>::max() - b) {
            cout << "Overflow!" << endl;
            return nullopt;
        }
    } else if (a<0 && b<0) {
        if (a < std::numeric_limits<T>::min() - b) {
            cout << "Overflow!" << endl;
            return nullopt;
        }
    }
    return (a+b);

}

int main() {
    int64_t a;
    int64_t b;
    cin >> a >> b;
    optional<int64_t> res = SafeAdd(a,b);
    if (res) {
        cout << res.value() << endl;
    }
}