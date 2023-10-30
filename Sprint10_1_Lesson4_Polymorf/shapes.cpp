// Макрос _USE_MATH_DEFINES необходим, чтобы при подключении <cmath> была объявлена константа M_PI
#define _USE_MATH_DEFINES
// Макрос _USE_MATH_DEFINES следует объявить ДО подключения других заголовочных файлов,
// которые могут подключить <cmath> неявно
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

using namespace std;
using namespace string_literals;

enum class Color { RED, GREEN, BLUE };

ostream& operator<<(ostream& out, Color color) {
    switch (color) {
        case Color::RED:
            out << "red"s;
            break;
        case Color::GREEN:
            out << "green"s;
            break;
        case Color::BLUE:
            out << "blue"s;
            break;
    }
    return out;
}

class Shape {
    // Напишите реализацию самостоятельно
public:
    Shape (Color color) :
        color_(color)
    {
    }

    Color GetColor() const {
        return color_;
    }

    void SetColor(Color col) {
        color_ = col;
    }

    virtual double GetArea() const {
        return 0.0;
    }

    virtual std::string GetType() const {
        return "Shape"s;
    }

private:
    Color color_;

};

class Rectangle : public Shape {
    // Напишите реализацию самостоятельно
public:
    Rectangle(double w, double h, Color col) :
        Shape(col),
        width_(w),
        height_(h)
    {
    }

    double GetWidth() const {
        return width_;
    }

    double GetHeight() const {
        return height_;
    }

    void SetWidth(double w) {
        width_ = w;
    }

    void SetHeight(double h) {
        height_ = h;
    }

    void SetSize(double w, double h) {
        SetWidth(w);
        SetHeight(h);
    }

    std::string GetType() const override {
        return "Rectangle"s;
    }

    double GetArea() const override {
        return width_*height_;
    }

private:
    double width_;
    double height_;
};

class Circle : public Shape {
    // Напишите реализацию самостоятельно
public:
    Circle(double rad, Color col) :
        Shape(col),
        radius_(rad)
    {
    }

    void SetRadius(double rad) {
        radius_ = rad;
    }

    double GetRadius() const {
        return radius_;
    }

    std::string GetType() const override {
        return "Circle"s;
    }

    double GetArea() const override {
        return M_PI*pow(radius_,2);
    }

private:
    double radius_;
};

// Возвращает суммарную площадь фигур, указатели на которые находятся в переданной коллекции collection
template <typename ShapeCollection>
double CalcSumArea(const ShapeCollection& collection) {
    double sum = 0;
    for (auto fig : collection) {
        sum += fig->GetArea();
    }
    return sum;
}

void PrintShapeInfo(const Shape& shape) {
    cout << shape.GetType() << ": color: "s << shape.GetColor() << ", area:"s << shape.GetArea() << endl;
}

int main() {
    Circle c{10.0, Color::RED};
    Rectangle r{10, 20, Color::BLUE};
    Shape sh{Color::GREEN};

    const Shape* shapes[] = {&c, &r, &sh};

    assert(sh.GetType() == "Shape"s);
    assert(c.GetType() == "Circle"s);
    assert(r.GetType() == "Rectangle"s);

    assert(sh.GetColor() == Color::GREEN);
    assert(c.GetColor() == Color::RED);
    assert(r.GetColor() == Color::BLUE);
    sh.SetColor(Color::BLUE);
    c.SetColor(Color::GREEN);
    r.SetColor(Color::RED);
    assert(sh.GetColor() == Color::BLUE);
    assert(c.GetColor() == Color::GREEN);
    assert(r.GetColor() == Color::RED);

    assert(std::abs(r.GetArea() - 200.0) < 1e-5);
    assert(std::abs(c.GetArea() - 314.15) < 1e-2);
    c.SetRadius(1.0);
    assert(std::abs(c.GetArea() - 3.1415) < 1e-4);
    r.SetSize(5, 7);
    assert(r.GetWidth() == 5);
    assert(r.GetHeight() == 7);
    assert(std::abs(r.GetArea() - 35.0) < 1e-5);

    assert(abs(CalcSumArea(shapes) - 38.1416) < 1e-4);

    for (const Shape* shape : shapes) {
        PrintShapeInfo(*shape);
    }
    cout << "Total area: " << CalcSumArea(shapes) << endl;
}