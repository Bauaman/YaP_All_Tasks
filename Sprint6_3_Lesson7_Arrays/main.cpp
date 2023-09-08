#include <cassert>
#include <vector>

template <typename T>
void ReverseArray(T* start, size_t size) {
    for (size_t i=0; i != size/2; ++i) {
        T temp = start[size-i-1];
        start[size -i-1] = start[i];
        start[i] = temp;
    }
}

int main() {
    using namespace std;
    
    vector<int> v = {1, 2};
    ReverseArray(v.data(), v.size());
    assert(v == (vector<int>{2, 1}));
}