#include <array>
#include <iostream>
#include <string>

#include "identity_document.h"
#include "passport.h"
#include "driving_licence.h"
#include "international_driving_licence.h"
#include "travel_pack.h"

using namespace std::string_literals;
using namespace std::string_view_literals;

void PrintInfo(IdentityDocument doc) {
    doc.PrintID();
}

void PrintInfo() {
    IdentityDocument::PrintUniqueIDCount();
}

void PrintInfo(const Passport& pass) {
    pass.PrintVisa("France"s);
}

void PrintDrivingLicence(DrivingLicence dr_lic) {
    dr_lic.PrintID();
}

IdentityDocument PrintInfo(int i) {
    Passport pass;
    std::cout << "PrintInfo("sv << i << ")"sv << std::endl;
    pass.PrintID();
    return pass;
}

int main() {
    std::cout << "Test1"sv << std::endl;
    Passport pass;
    PrintInfo(pass);
    PrintInfo(3).PrintID();

    std::cout << "Test2"sv << std::endl;
    std::array<IdentityDocument*, 3> docs = { (IdentityDocument*)(new Passport()), (IdentityDocument*)(new DrivingLicence()), (IdentityDocument*)(new Passport()) };
    for (const auto* doc : docs) {
        doc->PrintID();
    }

    for (size_t i = 0; i < docs.size(); ++i) {
        docs[i]->Delete();
    }

    std::cout << "Test3"sv << std::endl;
    std::array<IdentityDocument, 3> docs2 = {Passport(), DrivingLicence(), IdentityDocument()};

    for (size_t i = 0; i < docs2.size(); ++i) {
        docs2[i].PrintID();
    }

    PrintInfo(docs2[0]);

    std::cout << "Test4"sv << std::endl;
    IdentityDocument::PrintUniqueIDCount();

    std::cout << "Test5"sv << std::endl;
    Passport pass2;
    pass2.PrintUniqueIDCount();
}