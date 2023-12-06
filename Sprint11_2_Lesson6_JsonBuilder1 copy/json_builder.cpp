#include "json_builder.h"

namespace json {

    bool Builder::CheckIfCanAdd() {
        return (nodes_stack_.empty() || nodes_stack_.back()->IsString() || nodes_stack_.back()->IsArray()); 
    }

    Builder& Builder::StartDict() {
        if (CheckIfCanAdd) {
            nodes_stack_.emplace_back(new Dict{});
            ++counter_dict;
        }
        return *this;
    }

    Builder& Builder::StartArray() {
        if (CheckIfCanAdd) {
            nodes_stack_.emplace_back(new Array{});
            ++counter_arr;
        }
        return *this;
    }

    Builder& Builder::Key(std::string key) {
        if ((nodes_stack_.empty() && !nodes_stack_.back()->IsDict()) || finished) {
            throw std::logic_error("Cannot add key to non dict node");
        }
        nodes_stack_.emplace_back(new Node(key));
        return *this;
    }

    Builder& Builder::Value(json::Node::Value value) {
        if (!CheckIfCanAdd) {
            throw std::logic_error("Cannot add value to node");
        }
        if (nodes_stack_.empty()) {
            nodes_stack_.emplace_back(new Node::Value(std::move(value)));
            finished = true;
            return *this;
        }
        if (nodes_stack_.back()->IsString()) {
            nodes_stack_.emplace_back(new Node::Value(std::move(value)));
            return *this;
        }
        if (nodes_stack_.back()->IsArray()) {
            Array arr(nodes_stack_.back()->AsArray());
            arr.emplace_back(Node::Value(std::move(value)));
            delete nodes_stack_.back();
            nodes_stack_.pop_back();
            nodes_stack_.emplace_back(new Node::Value(std::move(arr)));
            return *this;
        }
    }

    Builder& Builder::EndArray() {
        if (counter_arr > 0 && !finished && nodes_stack_.back()->IsArray()) {
            Array arr(nodes_stack_.back()->AsArray());
            delete nodes_stack_.back();
            nodes_stack_.pop_back();
            if (nodes_stack_.back()->IsString()) {
                std::string str(nodes_stack_.back()->AsString());
                delete nodes_stack_.back();
                nodes_stack_.pop_back();
                if (nodes_stack_.back()->IsDict()) {
                    Dict dict(nodes_stack_.back()->AsDict());
                    delete nodes_stack_.back();
                    nodes_stack_.pop_back();
                    dict.emplace(str, arr);
                    nodes_stack_.emplace_back(new Node::Value(std::move(dict)));
                    --counter_arr;                 
                }
            }
        }
    }

    Builder& Builder::EndDict() {

    }

} //namespace json
