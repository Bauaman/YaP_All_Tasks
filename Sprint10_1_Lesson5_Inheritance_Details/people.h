#pragma once

#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person& /*person*/, int /*old_value*/, int /*new_value*/) {
        // Реализация метода базового класса ничего не делает
    }

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person {
public:
    Person(const std::string& name, int age) :
        name_(name),
        age_(age)
    {
    }

    virtual ~Person() = default;

    const std::string& GetName() const {
        return name_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        // Напишите реализацию самостоятельно
        observer_ = observer;
    }

    int GetAge() const {
        return age_;
    }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    virtual void Dance() {
        // Напишите тело метода самостоятельно
        ++dances_;
        SetSatisfaction(GetSatisfaction() + 1);
    }

    int GetSatisfaction() const {
        return satisfaction_;
    }

    int GetDanceCount() const {
        return dances_;
    }

    // Прожить день. Реализация в базовом классе ничего не делает
    virtual void LiveADay() {
    }

protected:
    void SetSatisfaction(int value) {
        if (satisfaction_ != value) {
            int old_s_value = satisfaction_;
            satisfaction_ = value;
            if (observer_) {
                observer_->OnSatisfactionChanged(*this, old_s_value, satisfaction_);
            }
        }

    }

private:
    std::string name_;
    PersonObserver* observer_ = nullptr;
    int satisfaction_ = 100;
    int age_;
    int dances_ = 0;
};

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person {
public:
    Worker(const std::string& name, int age) :
        Person(name,age)
    {
    }

    virtual void Dance() {
        const int age = GetAge();
        if (age >= 30 && age <=40) {
            Person::Dance();
            SetSatisfaction(GetSatisfaction() + 1);
        } else {
            Person::Dance();
        }
    }

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1
    // День рабочего проходит за работой
    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void LiveADay() override {
        Work();
    }

    void Work() {
        // Напишите тело метода самостоятельно
        SetSatisfaction(GetSatisfaction() - 5);
        ++work_done_;
    }

    // Возвращает значение счётчика сделанной работы
    int GetWorkDone() const {
        return work_done_;
    }

private:
    int work_done_ = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, int age) :
        Person(name, age)
    {
    }

    virtual void Dance() {
        Person::Dance();
    }

    // День студента проходит за учёбой
    void LiveADay() override {
        Study();
    }
    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        SetSatisfaction(GetSatisfaction() - 3);
        ++knowledge_level_;
    }

    // Возвращает уровень знаний
    int GetKnowledgeLevel() const {
        return knowledge_level_;
    }
private:
    int knowledge_level_ = 0;
};