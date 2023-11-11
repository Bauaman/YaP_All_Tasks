#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json {

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node;
    using Array = std::vector<Node>;
    using Dict = std::map<std::string, Node>;

    class Node final{
    public:
        using Value = std::variant<std::nullptr_t, Array, Dict, double, bool, int, std::string>;

        Node() = default;
        Node(std::nullptr_t);
        Node(std::string);
        Node(int);
        Node(bool);
        Node(double);
        Node(Array);
        Node(Dict);

        bool IsNull() const;
        bool IsDict() const;
        bool IsString() const;
        bool IsDouble() const;
        bool IsArray() const;
        bool IsBool() const;
        
        const std::string& AsString() const;
        const Dict& AsDict() const;
        double AsDouble() const;
        const Array& AsArray() const;
        bool AsBool() const;

        const Value& GetValue() const;
        
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const; 

    private:
        Value value_;

    };

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;
    
        bool operator==(const Document& rhs) const;
        bool operator!=(const Document& rhs) const;
    
    private:
        Node root_;    
    };

    Document Load(std::istream& input);

}//namespace json