#include "ini.h"

using namespace std;

namespace ini {

    Section& Document::AddSection(string name) {
        return sections_[name];
    }

    const Section& Document::GetSection(const string& name) const {
        static Section empty = {};
        if (!sections_.count(name) || sections_.at(name).empty()) {
            return empty;
        }
        return sections_.at(name);
    }

    size_t Document::GetSectionCount() const {
        return sections_.size();
    }

    void ClearString(string& str) {
        if (!str.empty()) str = str.substr(str.find_first_not_of(' '), str.find_last_not_of(' ') - str.find_first_not_of(' ') + 1);
    }

    Document Load (istream& input) {
        Document result;
        string line;
        Section* sect = nullptr;
        while (getline(input, line))
        {
            ClearString(line);
            if (line[0] == '[') {
                string section_name = line.substr(1, line.find(']')-1);
                sect = &result.AddSection(section_name);
            } else if (!line.empty()) {
                auto pos = line.find_first_of('=');
                string left = line.substr(0,pos);
                string right = line.substr(pos+1);
                ClearString(left);
                ClearString(right);
                sect->insert({left,right});
            } else {
                continue;
            }
        }
        return result;
    }

}

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini