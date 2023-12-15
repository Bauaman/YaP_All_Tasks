#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>


struct Query {
    std::string type;
    int user = 0;
    int page_num = 0;
};

class Ebook {
public:
    Ebook() :
        page_number_for_users_(MAX_USERS_ +1 , 0),
        users_qty_for_page_(MAX_PAGES_ + 1, 0) 
    {
    }

    void ProcessQuery(const Query& q, std::ostream& out);

private:
    static const int MAX_USERS_ = 100000;
    static const int MAX_PAGES_ = 1000;

    void ProcessReadQuery(const Query& q);
    double ProcessCheerQuery(const Query& q) const;
    size_t GetUsersCount() const;

    std::vector<size_t> page_number_for_users_;
    std::vector<size_t> users_qty_for_page_;
    size_t users_total_ = 0;
};

void Ebook::ProcessQuery(const Query& q, std::ostream& out) {
    if (q.type == "CHEER") {
        out << std::setprecision(6) << ProcessCheerQuery(q) << std::endl;
        return;
    }
    if (q.type == "READ") {
        ProcessReadQuery(q);
        return;
    }
}

size_t Ebook::GetUsersCount() const {
    return users_total_;
}

void Ebook::ProcessReadQuery(const Query& q) {

    if (page_number_for_users_[q.user] == 0) {
        ++users_total_;
    } else {
        size_t old_page_number_for_user = page_number_for_users_[q.user];
        users_qty_for_page_[old_page_number_for_user] -=1;
    }
    page_number_for_users_[q.user] = q.page_num;
    ++users_qty_for_page_[q.page_num];
}

double Ebook::ProcessCheerQuery(const Query& q) const {
    if (page_number_for_users_[q.user] == 0) {
        return 0;
    } else if (users_total_ == 1) {
        return 1;
    }
    size_t cur_page = page_number_for_users_[q.user];
    size_t users_qty = std::accumulate(users_qty_for_page_.begin(), users_qty_for_page_.begin()+cur_page, 0);
    return users_qty*1./(GetUsersCount() - 1)*1.;
}


int ParseStringToNum(std::string str) {
    int res = 0;
    for (size_t i = 0; i < str.size(); ++i) {
        res += (str[i] - '0')*std::pow(10, str.size()-i-1);
    }
    return res;
}

void Trim(std::string& query) {
    const auto start_str = query.find_first_not_of(' ');
    if (start_str == query.npos) {
        return;
    }
    query = query.substr(start_str, query.find_last_not_of(' ') + 1 - start_str);
}

Query ParseLine(std::string& query) {
    Query q;
    Trim(query);
    auto space_pos = query.find(' ');
    auto not_space = query.find_first_not_of(' ', space_pos);
    q.type = query.substr(0, space_pos);
    std::string rest = query.substr(not_space);
    if (q.type == "CHEER") {
        q.user = ParseStringToNum(rest);
    } else if (q.type == "READ") {
        space_pos = rest.find(' ');
        q.user = ParseStringToNum(rest.substr(0, space_pos));
        not_space = rest.find_first_not_of(' ', space_pos);
        q.page_num = ParseStringToNum(rest.substr(not_space));
    }
    return q;
}

void ReadQueries(std::istream& input, Ebook& book) {
    int query_qty = 0;
    std::string qq;
    std::getline(input, qq);
    query_qty = ParseStringToNum(qq);
    for (size_t i=0; i<query_qty; ++i) {
        std::string query;
        std::getline(input, query);
        Query q = ParseLine(query);
        book.ProcessQuery(q, std::cout);
    }
}

int main() {
    Ebook book;
    ReadQueries(std::cin, book);
}
