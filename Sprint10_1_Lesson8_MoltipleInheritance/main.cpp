#define _USE_MATH_DEFINES
#include "svg.h"

#include <cmath>

using namespace std::literals;
using namespace svg;

/*
Пример использования библиотеки. Он будет компилироваться и работать, когда вы реализуете
все классы библиотеки.
*/
namespace {

svg::Polyline CreateStar(svg::Point center, double outer_rad, double inner_rad, int num_rays) {
    using namespace svg;
    Polyline polyline;
    for (int i = 0; i <= num_rays; ++i) {
        double angle = 2 * M_PI * (i % num_rays) / num_rays;
        polyline.AddPoint({center.x + outer_rad * sin(angle), center.y - outer_rad * cos(angle)});
        if (i == num_rays) {
            break;
        }
        angle += M_PI / num_rays;
        polyline.AddPoint({center.x + inner_rad * sin(angle), center.y - inner_rad * cos(angle)});
    }
    return polyline;
}
/*
// Выводит приветствие, круг и звезду
void DrawPicture() {
    Document doc;
    doc.Add(Circle().SetCenter({20, 20}).SetRadius(10));
    doc.Add(Text()
                .SetFontFamily("Verdana"s)
                .SetPosition({35, 20})
                .SetOffset({0, 6})
                .SetFontSize(12)
                .SetFontWeight("bold"s)
                .SetData("Hello C++"s));
    doc.Add(CreateStar({20, 50}, 10, 5, 5));
    doc.Render(std::cout);
}
*/
}  // namespace

namespace shapes {

    class Triangle : public svg::Drawable {
    public:
        Triangle(svg::Point p1, svg::Point p2, svg::Point p3) :
            p1_(p1),
            p2_(p2),
            p3_(p3)
        {
        }
    
        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Polyline().AddPoint(p1_).AddPoint(p2_).AddPoint(p3_).AddPoint(p1_));
    }
    
    private:
        svg::Point p1_, p2_, p3_;
    };

    class Snowman : public svg::Drawable {
    public:
        Snowman(svg::Point center_low, double r1) :
            center_lower_(center_low),
            r_(r1)      
        {
        }

        void Draw(svg::ObjectContainer& container) const override {
            container.Add(svg::Circle().SetCenter({center_lower_.x, center_lower_.y+5*r_}).SetRadius(2*r_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
            //svg::Point center2 = {center_lower_.x,center_lower_.y+3*r_};
            //svg::Point center3 = {center_lower_.x,center_lower_.y+5*r_};
            container.Add(svg::Circle().SetCenter({center_lower_.x, center_lower_.y+2*r_}).SetRadius(1.5*r_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
            container.Add(svg::Circle().SetCenter(center_lower_).SetRadius(r_).SetFillColor("rgb(240,240,240)").SetStrokeColor("black"));
        }    
    private:
        svg::Point center_lower_;
        double r_;
    };

    class Star : public svg::Drawable {
    public:
        Star(svg::Point center, double outer_rad, double inner_rad, int num_rays) :
            center_(center),
            outer_rad_(outer_rad),
            inner_rad_(inner_rad),
            num_rays_(num_rays)
        {
        }

        void Draw(svg::ObjectContainer& container) const override {
            container.Add(CreateStar(center_,outer_rad_,inner_rad_,num_rays_).SetFillColor("red").SetStrokeColor("black"));
        }
    private:
        svg::Point center_;
        double outer_rad_;
        double inner_rad_;
        int num_rays_;
    };

} //namespace Shapes


template <typename DrawableIterator>
void DrawPicture(DrawableIterator begin, DrawableIterator end, svg::ObjectContainer& target) {
    for (auto it = begin; it != end; ++it) {
        (*it)->Draw(target);
    }
}

template <typename Container>
void DrawPicture(const Container& container, svg::ObjectContainer& target) {
    using namespace std;
    DrawPicture(begin(container), end(container), target);
}

int main() {
    using namespace svg;
    using namespace shapes;
    using namespace std;

    vector<unique_ptr<svg::Drawable>> picture;
    picture.emplace_back(make_unique<Triangle>(Point{100, 20}, Point{120, 50}, Point{80, 40}));
    picture.emplace_back(make_unique<Star>(Point{50.0, 20.0}, 10.0, 4.0, 5));
    picture.emplace_back(make_unique<Snowman>(Point{30, 20}, 10.0));

    svg::Document doc;
    DrawPicture(picture, doc);

    const Text base_text =  //
        Text()
            .SetFontFamily("Verdana"s)
            .SetFontSize(12)
            .SetPosition({10, 100})
            .SetData("Happy New Year!"s);
    doc.Add(Text{base_text}
                .SetStrokeColor("yellow"s)
                .SetFillColor("yellow"s)
                .SetStrokeLineJoin(StrokeLineJoin::ROUND)
                .SetStrokeLineCap(StrokeLineCap::ROUND)
                .SetStrokeWidth(3));
    doc.Add(Text{base_text}.SetFillColor("red"s));

    doc.Render(cout);
} 