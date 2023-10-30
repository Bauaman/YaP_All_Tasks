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
        out << "<points=\""sv;
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
    }

    Text& Text::SetOffset(Point offset) {
        text_.offset = offset;
    }

    Text& Text::SetFontSize(uint32_t size) {
        text_.size = size;
    }

    Text& Text::SetFontFamily(std::string font_family) {
        text_.font_family = font_family;
    }

    Text& Text::SetFontWeight(std::string font_weight) {
        text_.font_weight = font_weight;
    }

    Text& Text::SetData(std::string data) {
        text_.data = data;
    }

    void Text::RenderObject(const RenderContext& context) const {
        auto& out = context.out;
        out << "<text x=\""sv << text_.pos.x << "\" y=\"" << text_.pos.y;
        out << "\" dx=\"" << text_.offset.x << "\" dy=\"" << text_.offset.y;
        out << "\" font-size=\"" << text_.size << "\" font-family=\"" << text_.font_family;
        out << "\" font-weight=\"" << text_.font_weight << "\">" << text_.data;
        out << "</text>"; 
    }
    
    void Document::AddPtr(std::unique_ptr<Object>&& obj) {
        objects_.emplace_back(std::move(obj));
    }

    void Document::Render(std::ostream& out) const {
        
    }

}  // namespace svg