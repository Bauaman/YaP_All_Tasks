#include <algorithm>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "log_duration.h"

using namespace std;

class RandomContainer {
public:
    void Insert(int val) {
        values_pool_.push_back(val);
        values_to_index_.emplace(std::make_pair(val, values_pool_.size()-1));
    }
    void Remove(int val) {
        auto it = values_to_index_.find(val);
        size_t position = values_to_index_.at(val);
        values_pool_[position] = values_pool_.back();
        values_to_index_.at(values_pool_.back()) = position;
        values_to_index_.erase(it);
        values_pool_.pop_back();

    }
    bool Has(int val) const {
        return values_to_index_.count(val);
    }
    int GetRand() const {
        uniform_int_distribution<int> distr(0, values_pool_.size() - 1);
        int rand_index = distr(engine_);
        return values_pool_[rand_index];
    }

private:
    unordered_map<int, size_t> values_to_index_; 
    vector<int> values_pool_;
    mutable mt19937 engine_;
};

int main() {
    RandomContainer container;
    int query_num = 0;
    cin >> query_num;
    {
        LOG_DURATION("Requests handling"s);
        for (int query_id = 0; query_id < query_num; query_id++) {
            string query_type;
            cin >> query_type;
            if (query_type == "INSERT"s) {
                int value = 0;
                cin >> value;
                container.Insert(value);
            } else if (query_type == "REMOVE"s) {
                int value = 0;
                cin >> value;
                container.Remove(value);
            } else if (query_type == "HAS"s) {
                int value = 0;
                cin >> value;
                if (container.Has(value)) {
                    cout << "true"s << endl;
                } else {
                    cout << "false"s << endl;
                }
            } else if (query_type == "RAND"s) {
                cout << container.GetRand() << endl;
            }
        }
    }
}