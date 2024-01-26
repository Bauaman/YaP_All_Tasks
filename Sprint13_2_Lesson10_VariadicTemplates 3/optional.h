#include <stdexcept>
#include <utility>

// Исключение этого типа должно генерироватся при обращении к пустому optional
class BadOptionalAccess : public std::exception {
public:
    using exception::exception;

    virtual const char* what() const noexcept override {
        return "Bad optional access";
    }
};

template <typename T>
class Optional {
public:
    Optional() = default;
    Optional(const T& value) {
        ptr_ = new (&data_[0]) T(value);
        is_initialized_ = true;
    }
    Optional(T&& value) {
        ptr_ = new (&data_[0]) T(std::move(value));
        is_initialized_ = true;
    }
    Optional(const Optional& other) {
        is_initialized_ = other.is_initialized_;
        if (is_initialized_) {
            ptr_ = new (&data_[0]) T(other.Value());
        }
    }
    Optional(Optional&& other) {
        is_initialized_ = other.is_initialized_;
        if (is_initialized_) {
            ptr_ = new (&data_[0]) T(std::move(other.Value()));
        }
    }

    Optional& operator=(const T& value) {
        if (is_initialized_) {
            *ptr_ = value;
        } else {
            ptr_ = new (&data_[0]) T(value);
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(T&& rhs) {
        if (is_initialized_) {
            *ptr_ = std::move(rhs);
        } else {
            ptr_ = new (&data_[0]) T(std::move(rhs));
            is_initialized_ = true;
        }
        return *this;
    }
    Optional& operator=(const Optional& rhs) {
        if (is_initialized_ && rhs.is_initialized_) {
            *ptr_ = rhs.Value();
        } else if (!is_initialized_){
            ptr_ = new (&data_[0]) T(rhs.Value());
            is_initialized_ = true;
        } else {
            Reset();
        }
        return *this;
    }
    Optional& operator=(Optional&& rhs) {
        if (is_initialized_ && rhs.is_initialized_) {
            *ptr_ = std::move(rhs.Value());
        } else if (!is_initialized_) {
            ptr_ = new (&data_[0]) T(std::move(rhs.Value()));
            is_initialized_ = true;
        } else {
            Reset();
        }
        return *this;
    }

    ~Optional() {
        Reset();
    }

    bool HasValue() const{
        return is_initialized_;
    }

    // Операторы * и -> не должны делать никаких проверок на пустоту Optional.
    // Эти проверки остаются на совести программиста
    T& operator*() {
        return *ptr_;
    }
    const T& operator*() const {
        return *ptr_;
    }
    T* operator->() {
        return ptr_;
    }
    const T* operator->() const {
        return ptr_;
    }

    // Метод Value() генерирует исключение BadOptionalAccess, если Optional пуст
    T& Value() {
        if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *ptr_;
    }

    const T& Value() const {
                if (!is_initialized_) {
            throw BadOptionalAccess();
        }
        return *ptr_;
    }

    void Reset() {
        if (is_initialized_) {
            ptr_->~T();
            ptr_=nullptr;
        }
        is_initialized_ = false;
    }
   
    template <typename...Args>
    void Emplace(Args&&... args) {
        if (HasValue()) {
            Reset();
            }
        ptr_ = new(&data_[0]) T (std::forward<Args>(args)...);
        is_initialized_ = true;
    }

private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) char data_[sizeof(T)];
    bool is_initialized_ = false;
    T* ptr_ = nullptr;
};