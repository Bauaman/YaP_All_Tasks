#pragma once

#include "identity_document.h"

#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

class Passport {
public:
    Passport() :
        idoc_(),
        expiration_date_(GetExpirationDate()) {
            Passport::SetVtable(this);
            std::cout << "Passport::Ctor()"sv << std::endl;
        }

    Passport(const Passport& other) :
        idoc_(other.idoc_),
        expiration_date_(other.expiration_date_) {
            Passport::SetVtable(this);
            std::cout << "Passport::CCtor()"sv << std::endl;
        }
    
    ~Passport() {
        IdentityDocument::SetVtable((IdentityDocument*)this);
        std::cout << "Passport::Dtor()"sv << std::endl;
    }
    using DeleteFunction = void(*)(Passport*);
    using PrintIdFunction = void(*)(const Passport*);

    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction id_this;
    };

    void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << GetID() << std::endl;
    }

    void PrintID() const {
        GetVtable()->id_this(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    static void SetVtable (Passport* obj) {
        *(Passport::Vtable**) obj = &Passport::VTABLE;
    }

    int GetID() const {
        return idoc_.GetID();
    }

    Vtable* GetVtable() const {
        return reinterpret_cast<Vtable*>(idoc_.GetVtable());
    }

    operator IdentityDocument() {
        return {idoc_};
    }

    void PrintUniqueIDCount() {
        IdentityDocument::PrintUniqueIDCount();
    }

private:
    IdentityDocument idoc_;
    static Passport::Vtable VTABLE;

    const struct tm expiration_date_;
    
    tm GetExpirationDate() {
        time_t t = time(nullptr);
        tm exp_date = *localtime(&t);
        exp_date.tm_year += 10;
        mktime(&exp_date);
        return exp_date;
    }

    static void PrintID(const Passport* obj) {
        std::cout << "Passport::PrintID() : "sv << obj->GetID();
        std::cout << " expiration date : "sv << obj->expiration_date_.tm_mday << "/"sv << obj->expiration_date_.tm_mon << "/"sv
                  << obj->expiration_date_.tm_year + 1900 << std::endl;
    }

    static void Delete(Passport* obj) {
        delete(obj);
    }
};

Passport::Vtable Passport::VTABLE = {Passport::Delete, Passport::PrintID};