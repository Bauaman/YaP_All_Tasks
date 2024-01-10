#include "json_builder.h"

namespace json {

    Builder::Builder() {
        Node* root_ptr = &root_;
        nodes_stack_.emplace_back(root_ptr);
    }

    DictContext Builder::StartDict() {
        Node* last_node = nodes_stack_.back();
        if (root_.IsNull()) {
            last_node->GetValue() = Dict();
        } else if (last_node->IsDict()) {
            if (!key_) throw std::logic_error("Cannot emplace node without key into dict");
            auto& dict = std::get<Dict>(last_node->GetValue());
            auto [positon, success] = dict.emplace(std::move(key_.value()), Dict());
            nodes_stack_.emplace_back(&positon->second);
            key_ = std::nullopt;
        } else if (last_node->IsArray()) {
            auto& arr = std::get<Array>(last_node->GetValue());
            arr.emplace_back(Dict());
            nodes_stack_.emplace_back(&arr.back());
        } else {
            throw std::logic_error("Invalid StartDict call");
        }
        return *this;
    }

    ArrayContext Builder::StartArray() {
        Node* last_node = nodes_stack_.back();
        if (root_.IsNull()) {
            last_node->GetValue() = Array();
        } else if (last_node->IsDict()) {
            if (!key_) throw std::logic_error("Cannot emplace node without key into dict");
            auto& dict = std::get<Dict>(last_node->GetValue());
            auto [positon, success] = dict.emplace(std::move(key_.value()), Array());
            nodes_stack_.emplace_back(&positon->second);
            key_ = std::nullopt;           
        } else if (last_node->IsArray()) {
            auto& arr = std::get<Array>(last_node->GetValue());
            arr.emplace_back(Array());
            nodes_stack_.emplace_back(&arr.back());
        } else {
            throw std::logic_error("Invalid StartArray call");
        }
        return *this;
    }

    Builder& Builder::EndArray() {
        Node* last_node = nodes_stack_.back();
        if (!last_node->IsArray()) {
            throw std::logic_error("Last node is not an array");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    Builder& Builder::EndDict() {
        Node* last_node = nodes_stack_.back();
        if (!last_node->IsDict()) {
            throw std::logic_error("Last node is not a dictionary");
        }
        nodes_stack_.pop_back();
        return *this;
    }

    KeyContext Builder::Key(std::string key) {
        Node* last_node = nodes_stack_.back();
        if (!last_node->IsDict()) {
            throw std::logic_error("Invalid Key call: outside dictionary");
        }
        if (key_) throw std::logic_error("Invalid Key call: consequent key call");
        key_ = std::move(key);
        return *this;
    }

    Builder& Builder::Value(Node::Value value) {
        Node* last_node = nodes_stack_.back();
        if (last_node->IsDict()) {
            if (!key_) throw std::logic_error("Cannot emplace value into dict w/out key");
            auto& dict = std::get<Dict>(last_node->GetValue());
            auto [position, success] = dict.emplace(std::move(key_.value()), Node{});
            last_node = &position->second;
            last_node->GetValue() = std::move(value);
            key_ = std::nullopt;
        } else if (last_node->IsArray()) {
            auto& arr = std::get<Array>(last_node->GetValue());
            arr.emplace_back(GetNode(value));    
            last_node = &arr.back();
        } else if (root_.IsNull()) {
            root_.GetValue() = std::move(value);
        } else {
            throw std::logic_error("Invalid value call");
        }
        return *this;
    }

    Node Builder::Build() {
        if (root_.IsNull() || nodes_stack_.size()>1) {
            throw std::logic_error("Invalid build call");           
        }
        return root_;
    } 

    Node Builder::GetNode(Node::Value value) {
        if (std::holds_alternative<int>(value)) return Node(std::get<int>(value));
        if (std::holds_alternative<double>(value)) return Node(std::get<double>(value));
        if (std::holds_alternative<bool>(value)) return Node(std::get<bool>(value));
        if (std::holds_alternative<std::nullptr_t>(value)) return Node(std::get<std::nullptr_t>(value));
        if (std::holds_alternative<Dict>(value)) return Node(std::get<Dict>(value));
        if (std::holds_alternative<Array>(value)) return Node(std::get<Array>(value));
        if (std::holds_alternative<std::string>(value)) return Node(std::get<std::string>(value));

        return {};
    }

    Context::Context(Builder& builder) :
        bldr_(builder)
    {}

    KeyContext::KeyContext(Builder& buider) :
        Context(buider)
    {}

    DictContext KeyContext::Value(Node::Value value) {
        return DictContext(bldr_.Value(std::move(value)));
    }

    ArrayContext KeyContext::StartArray() {
        return bldr_.StartArray();
    }

    DictContext KeyContext::StartDict() {
        return bldr_.StartDict();
    }

    DictContext::DictContext(Builder& buider) :
        Context(buider)
    {}

    KeyContext DictContext::Key(std::string key) {
        return KeyContext(bldr_.Key(std::move(key)));
    }
    
    Builder& DictContext::EndDict() {
        return bldr_.EndDict();
    }

    ArrayContext::ArrayContext(Builder& buider) :
        Context(buider)
    {}

    ArrayContext ArrayContext::StartArray() {
        return bldr_.StartArray();
    }

    DictContext ArrayContext::StartDict() {
        return bldr_.StartDict();
    }

    ArrayContext ArrayContext::Value(Node::Value value) {
        return ArrayContext(bldr_.Value(std::move(value)));
    }

    Builder& ArrayContext::EndArray() {
        return bldr_.EndArray();
    }

    
}