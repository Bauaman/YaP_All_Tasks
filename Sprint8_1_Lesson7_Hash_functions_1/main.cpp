#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

using namespace std;

template <typename Hash>
int FindCollisions(const Hash& hasher, istream& text) {
    int collisions = 0;
    unordered_map<size_t, unordered_set<string>> hash_cont_;
    string word;
    
    while (text >> word) {
        size_t hash = hasher(word);
        auto [inserted_word, collision_bool] = hash_cont_[hash].insert(move(word));
        if (collision_bool && hash_cont_[hash].size() > 1) {
            collisions++;
        }
    }
    return collisions;
}


// Это плохой хешер. Его можно использовать для тестирования.
// Подумайте, в чём его недостаток
struct HasherDummy {
    size_t operator() (const string& str) const {
        size_t res = 0;
        for (char c : str) {
            res += static_cast<size_t>(c);
        }
        return res;
    }
};

struct DummyHash {
    size_t operator()(const string&) const {
        return 42;
    }
};

int main() {
    DummyHash dummy_hash;
    hash<string> good_hash;

    {
        istringstream stream("a b a b a b"s);
        cout << FindCollisions(dummy_hash, stream) << endl;
    }
    {
        istringstream stream("a b a b a b"s);
        cout << FindCollisions(good_hash, stream) << endl;
    }
}