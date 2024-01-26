#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>
#include <memory>
#include <algorithm>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;
    
    explicit RawMemory(size_t capacity) :
        buffer_(Allocate(capacity)),
        capacity_(capacity)
    {}

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory& rhs) = delete;
    
    RawMemory(RawMemory&& other) noexcept {
        Swap(other);
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }
    
    RawMemory& operator=(RawMemory&& rhs) noexcept {
        if (this != &rhs) {
            buffer_.~RawMemory();
            capacity_ = 0;
            Swap(rhs);
        }
        return *this;       
    }
    ~RawMemory() {
        Deallocate(buffer_);
    }

    T* operator+(size_t offset) noexcept {
        assert(offset <= capacity_);
        return buffer_ + offset;
    }
    const T* operator+(size_t offset) const noexcept {
        return const_cast<RawMemory&>(*this) + offset;
    }
    T& operator[](size_t index) noexcept {
        assert(index < capacity_);
        return buffer_[index];
    }
    const T& operator[](size_t index) const noexcept {
        return const_cast<RawMemory&>(*this)[index];
    }


    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }
    const T* GetAddress() const noexcept {
        return buffer_;
    }
    T* GetAddress() noexcept {
        return buffer_;
    }
    size_t Capacity() const {
        return capacity_;
    }

private:
    static T* Allocate(size_t n) {
        return n != 0 ? static_cast<T*>(operator new(n * sizeof(T))) : nullptr;
    }
    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
    }

    T* buffer_ = nullptr;
    size_t capacity_ = 0;
};

template <typename T>
class Vector {
public:
    using iterator = T*;
    using const_iterator = const T*;

    Vector() = default;

    explicit Vector(size_t size) :
        data_(size)
        , size_(size)
        {
            std::uninitialized_value_construct_n(data_.GetAddress(), size);
        }

    Vector(const Vector& other) :
        data_(other.size_)
        , size_(other.size_)
        {
            std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
        }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                /* Применить copy-and-swap */
                Vector rhs_copy(rhs);
                Swap(rhs_copy);
            }
            else {
                /* Скопировать элементы из rhs, создав при необходимости новые
                или удалив существующие */
                if (rhs.size_ < size_) {
                    std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + rhs.size_, data_.GetAddress());
                    std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
                }
                else {
                    std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + size_, data_.GetAddress());
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, data_.GetAddress() + size_);
                }
                size_ = rhs.size_;
            }
        }
        return *this;
    }

    Vector(Vector&& other) noexcept {
        Swap(other);
    }
    Vector& operator=(Vector&& rhs) noexcept {
        if (this != &rhs) {
            Swap(rhs);
        }
        return *this;
    }
    ~Vector() {
        std::destroy_n(data_.GetAddress(), size_);
    }
    iterator begin() noexcept {
        return data_.GetAddress();
    }
    iterator end() noexcept {
        return data_.GetAddress() + size_;
    }
    const_iterator begin() const noexcept {
        return data_.GetAddress();
    }
    const_iterator end() const noexcept {
        return data_.GetAddress() + size_;
    }
    const_iterator cbegin() const noexcept {
        return begin();
    }
    const_iterator cend() const noexcept {
        return end();
    }
    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        //auto dist = std::distance(begin(), pos);
        size_t dist = pos - begin();
        T* result = nullptr;
        if (size_ == Capacity()) {
            RawMemory<T> new_data(size_ == 0 ? 1 : size_*2);
            result = new(new_data + dist) T(std::forward<Args>(args)...);
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(begin(), dist - 1, new_data.GetAddress());
                std::uninitialized_move_n(begin() + dist + 1, size_ - dist, new_data.GetAddress() + dist+1);
            } else {
                std::uninitialized_copy_n(begin(), dist, new_data.GetAddress());
                std::uninitialized_copy_n(begin() + dist, size_ - dist, new_data.GetAddress() + dist+1);
            }
            std::destroy_n(begin(), size_);
            data_.Swap(new_data);
        } else {
            if (size_ !=0) {
                iterator it = end() - 1;
                new (data_ + size_) T(std::move(*it));
                std::move_backward(begin() + dist, end(), end() + 1);
                std::destroy_at(begin() + dist);
            }
            result = new (data_ + dist) T(std::forward<Args>(args)...);
        }
        ++size_;
        return result;
    }
    iterator Erase(const_iterator pos) {
        assert(pos >= begin() && pos < end());
        size_t shift = pos - begin();
        std::move(begin() + shift + 1, end(), begin() + shift);
        PopBack();
        return begin() + shift;
    }
    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }
    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }

    size_t Size() const noexcept {
        return size_;
    }
    size_t Capacity() const noexcept {
        return data_.Capacity();
    }
    const T& operator[](size_t index) const noexcept {
            return const_cast<Vector&>(*this)[index];
    }
    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }
    void Reserve(size_t new_capacity) {
        if (new_capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_data(new_capacity);
        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        } else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }
        std::destroy_n(data_.GetAddress(), size_);
        data_.Swap(new_data);
        }
    void Swap(Vector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
    }

    void Resize(size_t new_size) {
        if (new_size > size_) {
            Reserve(new_size);
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
        }
        else {
            std::destroy_n(data_.GetAddress() + new_size, size_ - new_size);
        }
        size_ = new_size;
    }
    template <typename Type>
    void PushBack(Type&& value) {
        EmplaceBack(std::forward<Type>(value));
    }
    void PopBack() /* noexcept */ {
        if (size_ != 0) {
            std::destroy_at(data_.GetAddress() + size_ - 1);
            --size_;
        }
    }
    template <typename... Args>
    T& EmplaceBack(Args&&... args) {
        T* result = nullptr;
        if (size_ == Capacity()) {
            RawMemory<T> new_data(size_ ==0 ? 1 : size_*2);
            result = new(new_data + size_) T(std::forward<Args>(args)...);
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            std::destroy_n(data_.GetAddress(), size_);
            data_.Swap(new_data);
        } else {
            result = new (data_+size_) T(std::forward<Args>(args)...);
        }
        ++size_;
        return *result;
    }
    
private:
    RawMemory<T> data_;
    size_t size_ = 0;
};

