#pragma once

#include "identity_document.h"

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class DrivingLicence {
public:

    DrivingLicence() :
        idoc_() {
            DrivingLicence::SetVtable(this);
            std::cout << "DrivingLicence::Ctor()"sv << std::endl;
        }

    DrivingLicence(const DrivingLicence& other) :
        idoc_(other.idoc_) {
            DrivingLicence::SetVtable(this);
            std::cout << "DrivingLicence::CCtor()"sv << std::endl;
        }

    ~DrivingLicence() {
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
        IdentityDocument::SetVtable((IdentityDocument*)this);
    }

    using DeleteFunction = void(*)(DrivingLicence*);
    using PrintIdFunction = void(*)(const DrivingLicence*);
    using GetIDFunction = int(*)(const DrivingLicence*);

    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction id_this;
        const GetIDFunction get_id_this;
    };

    static void SetVtable(DrivingLicence* obj) {
        *(DrivingLicence::Vtable**) obj = &DrivingLicence::VTABLE;
    }

    void PrintID() const {
        GetVtable()->id_this(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    Vtable* GetVtable() const {
        return reinterpret_cast<Vtable*>(idoc_.GetVtable());
    }

    int GetID() const {
        return GetVtable()->get_id_this(this);
    }

    operator IdentityDocument() {
        return {idoc_};
    }

private:
    IdentityDocument idoc_;
    static DrivingLicence::Vtable VTABLE;

    static void PrintID(const DrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->idoc_.GetID() << std::endl;
    }

    static void Delete(DrivingLicence* obj) {
        delete obj;
    }

    static int GetID(const DrivingLicence* obj) {
        return obj->idoc_.GetID();
    }
};

DrivingLicence::Vtable DrivingLicence::VTABLE = {DrivingLicence::Delete, DrivingLicence::PrintID, DrivingLicence::GetID};