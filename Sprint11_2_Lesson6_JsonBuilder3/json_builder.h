#include "json.h"

#include <memory>
#include <optional>
#include <vector>
#include <string>

namespace json {

    class Context;
    class KeyContext;
    class DictContext;
    class ArrayContext;

    class Builder {
    public:
        Builder();
        KeyContext Key(std::string key);
        Builder& Value(Node::Value value);
        DictContext StartDict();
        ArrayContext StartArray();
        Builder& EndDict();
        Builder& EndArray();
        Node Build();

    private:
        json::Node root_{nullptr};
        std::vector<Node*> nodes_stack_;
        std::optional<std::string> key_{std::nullopt};

        Node GetNode(Node::Value value);
    };

    class Context {
    public:
        explicit Context(Builder& builder);
    protected:
        Builder& bldr_;
    };

    class KeyContext : private Context {
    public:
        KeyContext(Builder& builder);

        DictContext Value(Node::Value value);
        ArrayContext StartArray();
        DictContext StartDict();
    };

    class DictContext : private Context {
    public: 
        DictContext(Builder& builder);

        KeyContext Key(std::string key);
        Builder& EndDict();
    };

    class ArrayContext : private Context {
    public: 
        ArrayContext(Builder& builder);

        ArrayContext Value(Node::Value value);
        ArrayContext StartArray();
        DictContext StartDict();
        Builder& EndArray();    
    };


} //namespace json


