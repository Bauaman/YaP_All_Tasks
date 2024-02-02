#pragma once

#include "identity_document.h"
#include "driving_licence.h"

#include <iostream>
#include <string>

using namespace std::string_view_literals;

class InternationalDrivingLicence {
public:
    InternationalDrivingLicence() :
        id_() {
            InternationalDrivingLicence::SetVtable(this);
            std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
        }

    InternationalDrivingLicence(const InternationalDrivingLicence& other) :
        id_(other.id_) {
            InternationalDrivingLicence::SetVtable(this);
            std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
        }

    ~InternationalDrivingLicence() {
        DrivingLicence::SetVtable((DrivingLicence*)this);
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
    }

    using DeleteFunction = void(*)(InternationalDrivingLicence*);
    using PrintIdFunction = void(*)(const InternationalDrivingLicence*);
    using GetIDFunction = int(*)(const InternationalDrivingLicence*);

    struct Vtable {
        DeleteFunction delete_this;
        const PrintIdFunction id_this;
        const GetIDFunction get_id_this;
    };

    static void SetVtable(InternationalDrivingLicence* obj) {
        *(InternationalDrivingLicence::Vtable**) obj = &InternationalDrivingLicence::VTABLE;
    }

    void PrintID() const {
        GetVtable()->id_this(this);
    }

    void Delete() {
        GetVtable()->delete_this(this);
    }

    Vtable* GetVtable() const {
        return reinterpret_cast<Vtable*>(id_.GetVtable());
    }

    int GetID() const {
        return GetVtable()->get_id_this(this);
    }

    operator DrivingLicence() {
        return id_;
    }

private:
    DrivingLicence id_;
    static InternationalDrivingLicence::Vtable VTABLE;

    static void PrintID(const InternationalDrivingLicence* obj) {
        std::cout << "DrivingLicence::PrintID() : "sv << obj->GetID() << std::endl;
    }

    static void Delete(InternationalDrivingLicence* obj) {
        delete obj;
    }

    static int GetID(const InternationalDrivingLicence* obj) {
        return obj->id_.GetID();
    }
};

InternationalDrivingLicence::Vtable InternationalDrivingLicence::VTABLE = {InternationalDrivingLicence::Delete, InternationalDrivingLicence::PrintID, InternationalDrivingLicence::GetID};