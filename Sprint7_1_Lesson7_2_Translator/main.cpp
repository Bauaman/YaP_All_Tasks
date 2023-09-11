#include <cassert>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <set>

using namespace std;

class Translator {
public:

    Translator() = default;

    void Add(string_view source, string_view target) {
        words_.insert((string)source);
        words_.insert((string)target);

        forward_[*words_.find(string(source))] = *words_.find(string(target));
        backward_[*words_.find(string(target))] = *words_.find(string(source));
    }
    string_view TranslateForward(string_view source) const {
        if (forward_.count(source) == 0) {
            return "";
        }
        return forward_.at(source);
    }

    string_view TranslateBackward(string_view target) const {
        if (backward_.count(target) == 0) {
            return "";
        }
        return backward_.at(target);
    }

private:

    set<string> words_;
    map<string_view, string_view> forward_;
    map<string_view, string_view> backward_;
    
};

void TestSimple() {
    Translator translator;
    translator.Add(string("okno"s), string("window"s));
    translator.Add(string("stol"s), string("table"s));

    assert(translator.TranslateForward("okno"s) == "window"s);
    assert(translator.TranslateBackward("table"s) == "stol"s);
    assert(translator.TranslateForward("table"s) == ""s);
}

int main() {
    TestSimple();
    return 0;
}