#include <iostream>
#include <math.h>
#include <string>

using namespace std;

struct Circle {
    double x;
    double y;
    double r;
};

struct Dumbbell {
    Circle circle1;
    Circle circle2;
    string text;
};

struct CircleHasher {
    size_t operator() (const Circle& circle) const {
        size_t h_x = d_hasher_(circle.x);
        size_t h_y = d_hasher_(circle.y);
        size_t h_r = d_hasher_(circle.r);
        
        return h_x + h_y * 37 + h_r * (37 * 37);
    }

private:
    std::hash<double> d_hasher_;
};

struct DumbbellHash {
// реализуйте хешер для Dumbbell
    size_t operator() (const Dumbbell& dumb) const {
        size_t c1 = c_hasher_(dumb.circle1);
        size_t c2 = c_hasher_(dumb.circle1);
        size_t t = s_hasher_(dumb.text); 

        return c1*pow(37,2) + c2*37 + t;

    }

private:
    CircleHasher c_hasher_;
    std::hash<string> s_hasher_;
};

int main() {
    DumbbellHash hash;
    Dumbbell dumbbell{{10, 11.5, 2.3}, {3.14, 15, -8}, "abc"s};
    cout << "Dumbbell hash "s << hash(dumbbell);
}