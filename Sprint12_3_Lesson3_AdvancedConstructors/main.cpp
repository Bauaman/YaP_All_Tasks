#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class RouteManager {
public:
    RouteManager(int buckets) {
        reachable_lists_.reserve(buckets);
    }

    void AddRoute(int start, int finish) {
        reachable_lists_[start].emplace(finish);
        reachable_lists_[finish].emplace(start);
    }


    int FindNearestFinish(int start, int finish) const {
        int result = abs(start - finish);
 
        if (!reachable_lists_.count(start)) {
            return result;
        }
        if (!reachable_lists_.at(start).empty()) {

            auto temp = reachable_lists_.at(start).lower_bound(finish);
            int elt = *temp;
            int prev_elt = *(std::prev(temp));
            int minimum_dist = min(abs(elt - finish), abs(prev_elt - finish));
            if (minimum_dist < result) {
                result = minimum_dist;
            }
        }
        return result;
    }

private:
    std::unordered_map<int, std::set<int>> reachable_lists_;

};

int main() {
    int query_count;
    cin >> query_count;

    RouteManager routes(query_count*2);

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;
        int start, finish;
        cin >> start >> finish;
        if (query_type == "ADD"s) {
            routes.AddRoute(start, finish);
        } else if (query_type == "GO"s) {
            cout << routes.FindNearestFinish(start, finish) << "\n"s;
        }
    }
}