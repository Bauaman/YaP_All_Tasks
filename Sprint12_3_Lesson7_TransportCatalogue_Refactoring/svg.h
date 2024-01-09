#pragma once

#include <cstdint>
#include <cmath>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

using namespace std::literals;

namespace svg {

    //using Color = std::string;
    // Объявив в заголовочном файле константу со спецификатором inline,
    // мы сделаем так, что она будет одной на все единицы трансляции,
    // которые подключают этот заголовок.
    // В противном случае каждая единица трансляции будет использовать свою копию этой константы
    //inline const Color NoneColor{"none"};

    enum class StrokeLineCap {
        BUTT,
        ROUND,
        SQUARE,
    };

    enum class StrokeLineJoin {
        ARCS,
        BEVEL,
        MITER,
        MITER_CLIP,
        ROUND,
    };

    std::ostream& operator<<(std::ostream& out, StrokeLineCap line_cap);
    std::ostream& operator<<(std::ostream& out, StrokeLineJoin line_join);

    struct Point {
        Point() = default;
        Point(double x_, double y_)
            : x(x_)
            , y(y_) {
        }
        double x = 0;
        double y = 0;
    };

    struct Rgb {
        
        Rgb() = default;
        Rgb(uint8_t red_, uint8_t green_, uint8_t blue_) :
            red(red_),
            green(green_),
            blue(blue_)
        {
        }

        uint8_t red = 0;
        uint8_t green = 0;
        uint8_t blue = 0;
    };

    struct Rgba : public Rgb {
        Rgba()
            : Rgb()
            , opacity(1.0) {
        }        
        Rgba(uint8_t red_, uint8_t green_, uint8_t blue_, double opacity_)
            : Rgb(red_, green_, blue_)
            , opacity(opacity_) {
        }
        
        double opacity;
    };

    using Color = std::variant<std::monostate, std::string, svg::Rgb, svg::Rgba>;
    inline const Color NoneColor{std::monostate()};

    std::ostream& operator<<(std::ostream& out, svg::Color color);
      
    /*
    * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
    * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
    */

    struct OstreamColorPrinter {
        std::ostream& out;

        void operator()(std::monostate) const {
            out << "none";
        }
        void operator()(std::string col) const {
            out << col;
        }
        void operator()(svg::Rgb col) const {
            out << "rgb("s << (int)col.red << "," << (int)col.green << "," << (int)col.blue << ")";
        }
        void operator()(svg::Rgba col) const {
            out << "rgba("s << (int)col.red << "," << (int)col.green << "," << (int)col.blue << "," << (double)col.opacity << ")";
        }
    };
   

    struct RenderContext {
        RenderContext(std::ostream& out_)
            : out(out_) {
        }
        RenderContext(std::ostream& out_, int indent_step_, int indent_ = 0)
            : out(out_)
            , indent_step(indent_step_)
            , indent(indent_) {
        }
        RenderContext Indented() const {
            return {out, indent_step, indent + indent_step};
        }
        void RenderIndent() const {
            for (int i = 0; i < indent; ++i) {
                out.put(' ');
            }
        }

        std::ostream& out;
        int indent_step = 0;
        int indent = 0;
    };

    /*
    * Абстрактный базовый класс Object служит для унифицированного хранения
    * конкретных тегов SVG-документа
    * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
    */
    class Object {
    public:
        void Render(const RenderContext& context) const;

        virtual ~Object() = default;

    private:
        virtual void RenderObject(const RenderContext& context) const = 0;

    };

    /*
    * Класс Circle моделирует элемент <circle> для отображения круга
    */

    class ObjectContainer {
    public:
        template <typename T>
        void Add(T obj) {
            /*objects_.emplace_back*/AddPtr(std::make_unique<T>(std::move(obj)));
        }

        virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;

    protected:
        ~ObjectContainer() = default;

    };

    template <typename Owner>
    class PathProps {
        public:
            Owner& SetFillColor(Color fill_color) {
                fill_color_ = std::move(fill_color);
                return AsOwner();
            }
            Owner& SetStrokeColor(Color stroke_color) {
                stroke_color_ = std::move(stroke_color);
                return AsOwner();
            }
            Owner& SetStrokeWidth(double stroke_width) {
                stroke_width_ = stroke_width;
                return AsOwner();
            }
            Owner& SetStrokeLineCap (StrokeLineCap line_cap) {
                line_cap_ = line_cap;
                return AsOwner();
            }
            Owner& SetStrokeLineJoin (StrokeLineJoin line_join) {
                line_join_ = line_join;
                return AsOwner();
            }
        
        protected:

            ~PathProps() = default;

            void RenderAttrs(std::ostream& out) const {
                if (fill_color_) {
                    out << " fill=\""sv;
                    std::visit(OstreamColorPrinter{out}, *fill_color_);
                    out << "\""sv;
                }
                if (stroke_color_) {
                    out << " stroke=\""sv;
                    std::visit(OstreamColorPrinter{out}, *stroke_color_);
                    out << "\""sv;
                }
                if (stroke_width_) {
                    out << " stroke-width=\""sv << *stroke_width_ << "\""sv;
                }
                if (line_cap_) {
                    out << " stroke-linecap=\""sv << *line_cap_ << "\""sv;
                }
                if (line_join_) {
                    out << " stroke-linejoin=\""sv << *line_join_ << "\""sv;
                }
            }

        private:
            Owner& AsOwner() {
                return static_cast<Owner&>(*this);
            }

            std::optional<Color> fill_color_;
            std::optional<Color> stroke_color_;
            std::optional<double> stroke_width_;
            std::optional<StrokeLineCap> line_cap_;
            std::optional<StrokeLineJoin> line_join_;
    };


    class Circle final : public Object, public PathProps<Circle> {
    public:
        Circle& SetCenter(Point center);
        Circle& SetRadius(double radius);

    private:
        void RenderObject(const RenderContext& context) const override;

        Point center_;
        double radius_ = 1.0;
    };

    //Интерфейс Drawable задает обхекты, которые можно нарисовать с помощью Graphics
    class Drawable{
    public:
        virtual ~Drawable() = default; 
        virtual void Draw(ObjectContainer& oc) const = 0;
    };

    /*
    * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
    */
    class Polyline final : public Object, public PathProps<Polyline> {
    public:
        // Добавляет очередную вершину к ломаной линии
        Polyline& AddPoint(Point point);

    private:
        void RenderObject(const RenderContext& context) const override;

        std::vector<Point> poly_points_;
    };

    /*
    * Класс Text моделирует элемент <text> для отображения текста
    */

    class Text final : public Object, public PathProps<Text> {
    public:
        // Задаёт координаты опорной точки (атрибуты x и y)
        Text& SetPosition(Point pos);

        // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
        Text& SetOffset(Point offset);

        // Задаёт размеры шрифта (атрибут font-size)
        Text& SetFontSize(uint32_t size);

        // Задаёт название шрифта (атрибут font-family)
        Text& SetFontFamily(std::string font_family);

        // Задаёт толщину шрифта (атрибут font-weight)
        Text& SetFontWeight(std::string font_weight);

        // Задаёт текстовое содержимое объекта (отображается внутри тега text)
        Text& SetData(std::string data);

        // Прочие данные и методы, необходимые для реализации элемента <text>
    private:

        void RenderObject(const RenderContext& context) const override;
         
        Point pos_ = {0.0,0.0};
        Point offset_ = {0.0,0.0};
        uint32_t size_ = 1;
        std::string font_family_;
        std::string font_weight_;
        std::string data_;
       
    };

    class Document : public ObjectContainer {
    public:

        // Добавляет в svg-документ объект-наследник svg::Object
        void AddPtr(std::unique_ptr<Object>&& obj) override;

        // Выводит в ostream svg-представление документа
        void Render(std::ostream& out) const;

    private:
        std::vector<std::unique_ptr<Object>> objects_;

    };

}  // namespace svg