#include "people.h"

#include <stdexcept>

using namespace std;

Person::Person(const string& name, int age, Gender gender) :
    name_(name),
    age_(age),
    gender_(gender)
    {
    }

const std::string& Person::GetName() const {
    return name_;
}

int Person::GetAge() const {
    return age_;
}

Gender Person::GetGender() const {
    return gender_;
}

Programmer::Programmer(const string& name, int age, Gender gender) :
    // Напишите тело конструктора
    Person(name,age,gender)
    {
    }

void Programmer::AddProgrammingLanguage(ProgrammingLanguage language) {
    // Заглушка, реализуйте метод самостоятельно
    language_.insert(language);
    can_prog_ = true;
}

bool Programmer::CanProgram(ProgrammingLanguage language) const {
    if (can_prog_) {
        if (language_.count(language) > 0) {
            return true;
        }
    }
    return false;
}

Worker::Worker(const string& name, int age, Gender gender) :
    // Заглушка, реализуйте конструктор самостоятельно
    Person(name,age,gender)
    {
    }

void Worker::AddSpeciality(WorkerSpeciality speciality) {
    // Заглушка, реализуйте метод самостоятельно
    spec_.insert(speciality);
    has_spec_ = true;
}

bool Worker::HasSpeciality(WorkerSpeciality speciality) const {
    // Заглушка, реализуйте метод самостоятельно
    if (has_spec_) {
        if (spec_.count(speciality) > 0) {
            return true;
        }
    }
    return false;
}

