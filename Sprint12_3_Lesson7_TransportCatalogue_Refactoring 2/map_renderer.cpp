#include "map_renderer.h"

std::vector<svg::Polyline> MapRenderer::RenderRouteLines (std::map<std::string_view, const Bus*> buses, const SphereProjector& sp) const {
    std::vector<svg::Polyline> res;
    size_t num = 0;
    for (auto [bus_name, bus] : buses) {
        if (bus->route_stops_.size() == 0) {
            continue;
        }
        svg::Polyline route;
        for (auto& stop : bus->route_stops_) {
            route.AddPoint(sp(stop->coord_));
        }
        route.SetFillColor("none");
        route.SetStrokeColor(render_settings_.color_palette_[num]);
        route.SetStrokeWidth(render_settings_.line_width_);
        route.SetStrokeLineCap(svg::StrokeLineCap::ROUND);
        route.SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
        res.push_back(std::move(route));
        
        if (num < render_settings_.color_palette_.size()-1) {
            ++num;
        } else {
            num = 0;
        }
    }
    return res;
}

std::vector<svg::Text> MapRenderer::RenderRouteLabels(std::map<std::string_view, const Bus*>& buses, const SphereProjector& sp) const {
    std::vector<svg::Text> res;
    size_t num = 0;   
    for (auto [bus_name, bus] : buses) {
        if (bus->route_stops_.size() == 0) {
            continue;
        }
        svg::Text bus_label = 
        svg::Text()
            .SetFontSize(render_settings_.bus_label_font_size_)
            .SetFontFamily("Verdana")
            .SetFontWeight("bold")
            .SetData(bus->bus_name_)
            .SetFillColor(render_settings_.color_palette_[num])
            .SetOffset(render_settings_.bus_label_offset_)
            .SetPosition(sp(bus->pivot_stop_->coord_));

        svg::Text bus_label_underlayer = bus_label;
        bus_label_underlayer
            .SetFillColor(render_settings_.underlayer_color_)
            .SetStrokeColor(render_settings_.underlayer_color_)
            .SetStrokeWidth(render_settings_.underlayer_width_)
            .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
            .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);



        if (!bus->is_round_) {
            if (bus->route_stops_[0] != bus->pivot_stop_) {
                geo::Coordinates final_stop_coord = bus->route_stops_[0]->coord_;
                svg::Text bus_label_underlayer_2 = bus_label_underlayer;
                svg::Text bus_label_2 = bus_label;
                bus_label_underlayer_2.SetPosition(sp(final_stop_coord));
                bus_label_2.SetPosition(sp(final_stop_coord));
                res.push_back(bus_label_underlayer_2);
                res.push_back(bus_label_2);
            }
        }
        res.push_back(bus_label_underlayer);
        res.push_back(bus_label);
        if (num < render_settings_.color_palette_.size()-1) {
            ++num;
        } else {
            num = 0;
        }
    }
    return res;
}

std::vector<svg::Circle> MapRenderer::RenderStopCircles(std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const {
    std::vector<svg::Circle> res;
    for (auto [stop_name, stop] : stops) {
            svg::Circle stop_c = svg::Circle()
                                        .SetCenter(sp(stop->coord_))
                                        .SetRadius(render_settings_.stop_radius_)
                                        .SetFillColor("white");
            res.push_back(stop_c);
    }
    return res;
}

std::vector<svg::Text> MapRenderer::RenderStopLabels(std::map<std::string_view, const Stop*>& stops, const SphereProjector& sp) const {
    std::vector<svg::Text> res;
    for (auto [stop_name, stop] : stops) {
            svg::Text stop_label = svg::Text()
                                        .SetOffset(render_settings_.stop_label_offset_)
                                        .SetFontSize(render_settings_.stop_label_font_size_)
                                        .SetFontFamily("Verdana")
                                        .SetFontWeight("")
                                        .SetData(stop->stop_name_)
                                        .SetPosition(sp(stop->coord_));
            svg::Text stop_label_underlayer = stop_label;
            stop_label_underlayer.SetFillColor(render_settings_.underlayer_color_)
                                 .SetStrokeColor(render_settings_.underlayer_color_)
                                 .SetStrokeWidth(render_settings_.underlayer_width_)
                                 .SetStrokeLineCap(svg::StrokeLineCap::ROUND)
                                 .SetStrokeLineJoin(svg::StrokeLineJoin::ROUND);
            stop_label.SetFillColor("black");
            res.push_back(stop_label_underlayer);
            res.push_back(stop_label);
    }
    return res;
}


svg::Document/*void*/ MapRenderer::GetSVGDoc(std::map<std::string_view, const Bus*>& buses, std::map<std::string_view, const Stop*>& stops) const {
    std::vector<geo::Coordinates> stops_coordinates;
    for (auto [bus_name, bus] : buses) {
        for (Stop* stop : bus->route_stops_) {
            stops_coordinates.push_back(stop->coord_);
        }
    }
    SphereProjector sp(stops_coordinates.begin(), 
                        stops_coordinates.end(), 
                        render_settings_.width_, 
                        render_settings_.heigth_, 
                        render_settings_.padding_);
    svg::Document doc;
    for (auto line : RenderRouteLines(buses, sp)) {
        doc.Add(line);
    }
    for (auto label : RenderRouteLabels(buses,sp)) {
        doc.Add(label);
    }
    for (auto circle : RenderStopCircles(stops, sp)) {
        doc.Add(circle);
    }
    for (auto stop_label : RenderStopLabels(stops, sp)) {
        doc.Add(stop_label);
    }
    //doc.Render(out);
    return doc;
}

    std::string MapRenderer::GetSVGString(std::map<std::string_view, const Bus*>& buses, std::map<std::string_view, const Stop*>& stops) const {
        svg::Document doc = GetSVGDoc(buses,stops);
        std::stringstream out_string;
        doc.Render(out_string);
        return out_string.str();
    }