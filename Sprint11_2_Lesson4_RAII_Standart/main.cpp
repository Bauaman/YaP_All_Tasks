#include <iostream>
#include <deque>
#include <map>
#include <memory>
#include <set>
#include <stdint.h>
#include <string>
#include <utility>

using namespace std;

struct Reservation{
    Reservation(int64_t time, int cl_id, int room_cnt, std::string hotel_name) :
        time_(time),
        hotel_name_(hotel_name),
        client_id_(cl_id),
        room_count_(room_cnt)
    {}
    int64_t time_;
    std::string hotel_name_;
    int client_id_;
    int room_count_;
};


class HotelManager {
public:
    void Book(int64_t time, int cl_id, int room_cnt, std::string hotel_name) {
        Reservation res (time,cl_id,room_cnt,hotel_name);
        hotels_[res.hotel_name_].push_back(res);
        last_booking_time_ = time;
    }   
    int ComputeClientCount(std::string hotel_name) {
        std::set<int> clients;
        int64_t last_day_begin = last_booking_time_ - 86400;
        if (!hotels_.empty()) {
            std::deque<Reservation> copy = hotels_.at(hotel_name);
            for (Reservation res : copy) {
                if (res.time_ > last_day_begin) {
                    clients.insert(res.client_id_);
                }
            }
            return clients.size();
        }
        return 0;
    }
    int ComputeRoomCount(std::string hotel_name) {
        int64_t last_day_begin = last_booking_time_ - 86400;
        int count = 0;
        if (!hotels_.empty()) {
            std::deque<Reservation> copy = hotels_.at(hotel_name);
            for (Reservation res : copy) {
                if (res.time_ > last_day_begin) {
                    count += res.room_count_;
                }
            }
            return count;
        }
        return 0;
    }

private:
    int64_t last_booking_time_;
    std::map<std::string, std::deque<Reservation>> hotels_;
};

int main() {
    HotelManager manager;

    int query_count;
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(time, client_id, room_count, hotel_name);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}