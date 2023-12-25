#include <iostream>
#include <list>
#include <set>
#include <string_view>
#include <vector>

using namespace std;

// Измените сигнатуру этой функции, чтобы она могла
// сливать не только векторы, но и любые другие контейнеры
//template <typename T>
//void Merge(const vector<T>& src1, const vector<T>& src2, ostream& out) {
template <typename InIter1, typename InIter2>
void Merge(InIter1 begin1, const InIter1 end1, InIter2 begin2, const InIter2 end2, ostream& out) {
    //size_t i1 = 0, i2 = 0;
    while (begin1 != end1 && begin2 != end2) {
        if (less<>()(*begin1, *begin2)) {
            out << *begin1++ << endl;
        } else {
            out << *begin2++ << endl;
        }
    }
    while (begin1 != end1) {
        out << *begin1++ << endl;
    }
    while (begin2 != end2) {
        out << *begin2++ << endl;
    }
}

// Реализуйте эти функции. Они должны вызывать ваш вариант функции Merge.
// Чтобы сократить кличество работы, можете реализовать вместо них одну шаблонную.
template <typename T, typename S>
void MergeSomething(const T& src1, const S& src2, ostream& out) {
    // ...
    Merge(src1.begin(), src1.end(), src2.begin(), src2.end(), out);
}


// Реализуйте эту функцию:
template <typename T>
void MergeHalves(const vector<T>& src, ostream& out) {
    auto mid = src.begin() + (src.size() + 1) / 2;
    Merge(src.begin(), mid, mid, src.end(), out);
    // Сделать Merge участка вектора от 0 до mid и от mid до конца.
    // Элемент с индексом mid включается во второй диапазон.
}

int main() {
    vector<int> v1{60, 70, 80, 90};
    vector<int> v2{65, 75, 85, 95};
    vector<int> combined{60, 70, 80, 90, 65, 75, 85, 95};
    list<double> my_list{0.1, 72.5, 82.11, 1e+30};
    string_view my_string = "ACNZ"sv;
    set<unsigned> my_set{20u, 77u, 81u};

    // пока функция MergeSomething реализована пока только для векторов
    cout << "Merging vectors:"sv << endl;
    MergeSomething(v1, v2, cout);

    cout << "Merging vector and list:"sv << endl;
    MergeSomething(v1, my_list, cout);

    cout << "Merging string and list:"sv << endl;
    MergeSomething(my_string, my_list, cout);

    cout << "Merging set and vector:"sv << endl;
    MergeSomething(my_set, v2, cout);

    cout << "Merging vector halves:"sv << endl;
    MergeHalves(combined, cout);
}