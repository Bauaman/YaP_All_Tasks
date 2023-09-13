#include "log_duration.h"

#include <deque>
#include <iostream>
#include <string>
#include <vector>

using namespace std::string_literals;

int main() {

    {
        std::vector<int> v1;
        LOG_DURATION("Vector 1000000: ");
        for (int i=0; i!=1000000; ++i) {
            v1.push_back(i);
        }
    }
    {
        std::deque<int> dq1;
        LOG_DURATION("Deque 1000000: ");
        for (int i=0; i!=1000000; ++i) {
            dq1.push_back(i);
        }
    }
    {
        std::vector<int> v2;
        LOG_DURATION("Vector 5000000: ");
        for (int i=0; i!=5000000; ++i) {
            v2.push_back(i);
        }
    }
    {
        std::deque<int> dq2;
        LOG_DURATION("Deque 5000000: ");
        for (int i=0; i!=5000000; ++i) {
            dq2.push_back(i);
        }
    }
    {
        std::vector<int> v3;
        LOG_DURATION("Vector 10000000: ");
        for (int i=0; i!=10000000; ++i) {
            v3.push_back(i);
        }
    }
    {
        std::deque<int> dq3;
        LOG_DURATION("Deque 10000000: ");
        for (int i=0; i!=10000000; ++i) {
            dq3.push_back(i);
        }
    }

    std::vector<std::string> oh_my_god = { "Oh "s, "my "s, "God"s };
    const std::string* ptr = &oh_my_god[0];
    const int exclamations = 1000;

    std::cout << *ptr << *(ptr + 1) << *(ptr + 2) << std::endl;

    for (int i = 0; i < exclamations; ++i) {
        oh_my_god.push_back("!");
    }

    std::cout << *ptr << *(ptr + 1) << *(ptr + 2) << std::endl; 


    
    std::deque<int> numbers = {1};
    auto it = numbers.begin();
    std::cout << *it << std::endl;

    for (int i = 0; i < 1024; ++i) {
        numbers.push_back(i);
    }
    // попытаемся вывести последний элемент из дека
    std::cout << *it << " " << *(it + numbers.size() - 1) << std::endl;

}