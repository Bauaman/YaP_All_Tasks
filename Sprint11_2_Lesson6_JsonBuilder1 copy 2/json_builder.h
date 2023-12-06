#include "json.h"

#include <memory>
#include <optional>
#include <vector>
#include <string>

namespace json {

    class Builder {
    public:
        Builder();
        Builder& Key(std::string key);
        Builder& Value(Node::Value value);
        Builder& StartDict();
        Builder& StartArray();
        Builder& EndDict();
        Builder& EndArray();
        Node Build();

    private:
        json::Node root_{nullptr};
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> key_{std::nullopt};

        Node GetNode(Node::Value value);
    };

} //namespace json


