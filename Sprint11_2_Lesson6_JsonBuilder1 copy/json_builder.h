#include "json.h"

#include <vector>
#include <string>

namespace json {

    class Builder {
    public:
        Builder& Key(std::string key);
        Builder& Value(json::Node::Value value);
        Builder& StartDict();
        Builder& StartArray();
        Builder& EndDict();
        Builder& EndArray();
        json::Node Build();

        bool CheckIfCanAdd();

    private:
        json::Node root_{nullptr};
        std::vector<json::Node*> nodes_stack_;
        size_t counter_arr = 0;
        size_t counter_dict = 0;
        bool finished = false;

    };

} //namespace json


