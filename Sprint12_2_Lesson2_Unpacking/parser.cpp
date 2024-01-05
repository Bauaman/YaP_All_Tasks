#include "parser.h"

// разработайте менеджер личного бюджета самостоятельно
const Date Parser::GetDateFrom() const {
    return from_;
}

const Date Parser::GetDateTo() const {
    return to_;
}

double Parser::GetValue() const {
    return modificator_value_;
}

void Parser::SetDateFrom(const Date from) {
    from_ = from;
}

void Parser::SetDateTo(const Date to) {
    to_ = to;
}

void Parser::SetModificatorValue(const double modificator) {
    modificator_value_ = modificator;
}

Query Parser::ParseQueryString(std::string_view query_string) {
    Query q;
    int space_first = query_string.find_first_of(' ');
    int space_last = query_string.find_last_of(' ');
    q.date_from = Date::FromString(query_string.substr(0, space_first));
    
    if (space_first != space_last) {
        q.date_to = Date::FromString(query_string.substr(space_first+1, space_last-space_first-1));
        q.modificator = std::stod(std::string(query_string.substr(space_last+1)));
    } else {
        q.date_to = Date::FromString(query_string.substr(space_first+1));
    }
    return q;
}

void ParserRead::ParseQuery(Query query) {
    SetDateFrom(query.date_from);
    SetDateTo(query.date_to);
}   

void ParserModify::ParseQuery(Query query) {
    SetDateFrom(query.date_from);
    SetDateTo(query.date_to);
    SetModificatorValue(query.modificator);
}