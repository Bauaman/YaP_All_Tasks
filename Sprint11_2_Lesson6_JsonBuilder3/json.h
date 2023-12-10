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

    class Node final : private std::variant<std::nullptr_t, bool, int, double, std::string, Dict, Array> {
    public:
        using variant::variant;
        using Value = variant; //std::variant<std::nullptr_t, Array, Dict, double, bool, int, std::string>;

/*
        Node() = default;
        Node(std::nullptr_t);
        Node(std::string);
        Node(int);
        Node(bool);
        Node(double);
        Node(Array);
        Node(Dict);
*/
        bool IsNull() const;
        bool IsDict() const;
        bool IsString() const;
        bool IsDouble() const;
        bool IsPureDouble() const;
        bool IsArray() const;
        bool IsBool() const;
        bool IsInt() const;
        
        const std::string& AsString() const;
        const Dict& AsDict() const;
        double AsDouble() const;
        const Array& AsArray() const;
        bool AsBool() const;
        int AsInt() const;

        const Value& GetValue() const;
        Value& GetValue();
        bool operator==(const Node& rhs) const;
        bool operator!=(const Node& rhs) const; 

    private:
        //Value value_;

    };

    class Document {
    public:
        Document() = default;
        explicit Document(Node root);

        const Node& GetRoot() const;
    
        bool operator==(const Document& rhs) const;
        bool operator!=(const Document& rhs) const;
    
    private:
        Node root_;    
    };

    Document Load(std::istream& input);

struct PrintContext {

    std::ostream& out;
    int indent_step = 4;
    int indent = 0;
    void PrintIndent() const {
        for (int i = 0; i<indent; ++i) {
            out.put(' ');
        }
    }
    PrintContext Indented() const {
        return {out, indent_step, indent_step+indent};
    }
};

struct ValuePrinter {
    PrintContext& ctx;
    //std::ostream& out;

    void operator()(std::nullptr_t);
    void operator()(std::string value);
    void operator()(int value);
    void operator()(double value);
    void operator()(bool value);
    void operator()(Array array);
    void operator()(Dict dict);
};


void Print(const Document& doc, std::ostream& output);
void Print(const Node& node, std::ostream& output);
}//namespace json