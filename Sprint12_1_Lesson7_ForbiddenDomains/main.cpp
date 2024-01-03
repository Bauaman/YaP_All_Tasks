#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(const std::string& str) :
        domain_(str)
    {}

    bool operator==(const Domain& rhs) const {
        return (domain_ == rhs.domain_);
    }

    bool IsSubdomain(const Domain& parentDomain) const {
        if (domain_.size() >= parentDomain.GetValue().size()) {
            return (equal(parentDomain.domain_.begin(), parentDomain.domain_.end(), domain_.begin()));
        }
        else {
            return false;
        }
    }

    std::string GetValue() const {
        return domain_;
    }

private:
    std::string domain_;
};

class DomainChecker {
public:
    template <typename InputIt>
    DomainChecker(InputIt begin, InputIt end) :
        forbidden_domains_(begin, end) {
        SortAndEraseDuplicates();
    }

    bool IsForbidden(const Domain& domain) const {
        if (forbidden_domains_.empty()) {
            return false;
        }
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, 
                                [](const Domain& dom, const Domain& rhs) {
                                    return dom.GetValue() < rhs.GetValue();
                                });
        size_t dist = std::distance(forbidden_domains_.begin(), it);
        if (dist != 0) {
            return domain.IsSubdomain(forbidden_domains_[dist - 1]);
        }
        else {
            return domain.IsSubdomain(forbidden_domains_[dist]);
        }
        return true;
    }

private:
    void SortAndEraseDuplicates() {
        std::sort(forbidden_domains_.begin(), forbidden_domains_.end(), 
                    [](const Domain& rhs, const Domain& lhs) {
                        return rhs.GetValue() < lhs.GetValue();
                    });
        auto last_unique = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
                            [](const Domain& lhs, const Domain& rhs) {
                                return rhs.IsSubdomain(lhs);
                            });
        forbidden_domains_.erase(last_unique, forbidden_domains_.end());
    }
    std::vector<Domain> forbidden_domains_;
};

template <typename Number>
std::vector<Domain> ReadDomains(istream& input, Number num) {
    std::vector<Domain> reverse_domains;
    for (Number i = 0; i < num; ++i) {
        std::string str;
        std::getline(input, str);
        std::reverse(str.begin(), str.end());
        str.push_back('.');
        reverse_domains.emplace_back(std::move(str));
    }
    return reverse_domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}