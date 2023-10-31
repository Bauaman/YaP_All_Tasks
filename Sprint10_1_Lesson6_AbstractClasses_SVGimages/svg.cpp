#include "svg.h"

namespace svg {

    using namespace std::literals;

    void Object::Render(const RenderContext& context) const {
        context.RenderIndent();

        // Делегируем вывод тега своим подклассам
        RenderObject(context);
        context.out << std::endl;
    }

    // ---------- Circle ------------------

    Circle& Circle::SetCenter(Point center)  {
        center_ = center;
        return *this;
    }

    Circle& Circle::SetRadius(double radius)  {
        radius_ = radius;
        return *this;
    }

    void Circle::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
        out << "r=\""sv << radius_ << "\" "sv;
        out << "/>"sv;
    }

    // ---------- PolyLine ------------------

    Polyline& Polyline::AddPoint(Point point) {
        poly_points_.push_back(point);
        return *this;
    }

    void Polyline::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<polyline points=\""sv;
        bool isFirst = true;
        for (Point p : poly_points_) {
            if (isFirst) {
                out << p.x << ","sv << p.y;
                isFirst = false;
            } else {
                out << " "sv << p.x << ","sv << p.y;
            }
        }
        out << "\" />"sv;
    }

    // ---------- Text ------------------
    Text& Text::SetPosition(Point pos) {
        text_.pos = pos;
        return *this;
    }

    Text& Text::SetOffset(Point offset) {
        text_.offset = offset;
        return *this;
    }

    Text& Text::SetFontSize(uint32_t size) {
        text_.size = size;
        return *this;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        text_.font_family = font_family;
        return *this;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        text_.font_weight = font_weight;
        return *this;
    }

    Text& Text::SetData(std::string data) {
        text_.data = data;
        return *this;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<text x=\""sv << text_.pos.x << "\" y=\""sv << text_.pos.y;
        out << "\" dx=\""sv << text_.offset.x << "\" dy=\""sv << text_.offset.y;
        out << "\" font-size=\""sv << text_.size;
        if (!text_.font_family.empty()) out << "\" font-family=\""sv << text_.font_family;
        if (!text_.font_weight.empty()) out << "\" font-weight=\""sv << text_.font_weight << "\">"sv << text_.data;
        out << "</text>"sv; 
    }
    
    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {
        RenderContext ctx {std::cout, 2, 2};
        out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>"sv << std::endl;
        out << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">"sv << std::endl;
        for (const auto& obj : objects_) {
            obj->Render(ctx);
        }
        out << "</svg>"sv;
    }

}  // namespace svg