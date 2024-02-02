#pragma once

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class IdentityDocument {
public:
    IdentityDocument() :
        unique_id_(++unique_id_count_)
        {
            IdentityDocument::SetVtable(this);
            std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
        }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    using DeleteFunction = void(*)(IdentityDocument*);
    using PrintIdFunction = void(*)(const IdentityDocument*);

    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction id_this;
    };

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    IdentityDocument(const IdentityDocument& other) :
        unique_id_(++unique_id_count_)
        {
            IdentityDocument::SetVtable(this);
            std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
        }

    void PrintID() const {
        GetVtable()->id_this(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    static void Delete(IdentityDocument* obj) {
        delete obj;
    }

    int GetID() const {
        return unique_id_;
    }

    Vtable* GetVtable() const{
        return vptr_;
    }

    static void SetVtable(IdentityDocument* obj) {
        *(Vtable**) obj = &IdentityDocument::VTABLE;
    }

    static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

private:
    IdentityDocument::Vtable* vptr_ = nullptr;
    static IdentityDocument::Vtable VTABLE;

    static int unique_id_count_;
    int unique_id_;

    static void PrintID(const IdentityDocument* obj) {
        std::cout << "IdentityDocument::PrintID() : "sv << obj->unique_id_ << std::endl;
    }
};

IdentityDocument::Vtable IdentityDocument::VTABLE = {IdentityDocument::Delete, IdentityDocument::PrintID};

int IdentityDocument::unique_id_count_ = 0;