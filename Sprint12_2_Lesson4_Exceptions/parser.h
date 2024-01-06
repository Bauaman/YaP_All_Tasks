#pragma once
#include "date.h"
#include <string_view>
#include <string>

struct Query {
    Date date_from;
    Date date_to;
    double modificator = 0;
};

class Parser {
public:
    virtual void ParseQuery(const Query query) = 0;

    void SetDateFrom(const Date from);
    void SetDateTo(const Date to);
    void SetModificatorValue(const double modificator);
    Query ParseQueryString(std::string_view query_string);

    const Date GetDateFrom() const;
    const Date GetDateTo() const;
    double GetValue() const;
    
private:
    
    Date from_;
    Date to_;
    double modificator_value_ =0.;
};

class ParserRead : public Parser {
public:
    ParserRead(std::string_view query_string) {
        Query query = ParseQueryString(query_string);
        ParseQuery(query);
    }
    void ParseQuery(const Query query) override;
};

class ParserModify : public Parser {
public:
    ParserModify(std::string_view query_string) {
        Query query = ParseQueryString(query_string);
        ParseQuery(query);
    }
    void ParseQuery(const Query query) override;
};