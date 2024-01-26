#pragma once
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <new>
#include <memory>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    RawMemory(const RawMemory&) = delete;
    RawMemory& operator=(const RawMemory&& rhs) = delete;

    RawMemory(RawMemory&& other) noexcept {
        Swap(other);
        other.buffer_ = nullptr;
        other.capacity_ = 0;
    }

    RawMemory& operator=(RawMemory&& rhs) noexcept {
        if (this != rhs) {
            buffer_.~RawMemory();
            capacity_ = 0;
            Swap(rhs);
        }
        return *this;
    }

    explicit RawMemory(size_t cap) :
        buffer_(Allocate(cap)),
        capacity_(cap)
    {}

    ~RawMemory() {
        Deallocate(buffer_);
    }

    T* operator+(size_t offset) noexcept {
        assert(offset <= capacity_);
        return buffer_ + offset;
    }

    const T* operator+(size_t offset) const noexcept {
        assert(offset <= capacity_);
        return const_cast<RawMemory&>(*this) + offset;
    }

    T& operator[](size_t index) noexcept {
        assert (index < capacity_);
        return buffer_[index];
    }

    const T& operator[](size_t index) const noexcept {
        assert (index < capacity_);
        return const_cast<RawMemory&>(*this)[index];
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

    void Swap(RawMemory& other) noexcept {
        std::swap(buffer_, other.buffer_);
        std::swap(capacity_, other.capacity_);
    }

private:

    static T* Allocate(size_t size) {
        return size != 0 ? static_cast<T*>(operator new(size * sizeof(T))) : nullptr;
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
        data_(size),
        size_(size)
    {
        std::uninitialized_value_construct_n(data_.GetAddress(), size_);
    }

    Vector(const Vector& other) :
        data_(other.Size()),
        size_(other.Size())
        {
            std::uninitialized_copy_n(other.data_.GetAddress(), size_, data_.GetAddress());
        }

    Vector (Vector&& other) {
        Swap(other);
    }

    Vector& operator=(const Vector& rhs) {
        if (this != &rhs) {
            if (rhs.size_ > data_.Capacity()) {
                Vector rhs_copy(rhs);
                Swap(rhs_copy);
            } else {
                if (rhs.size_ < size_) {
                    std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + rhs.size_, data_.GetAddress());
                    std::destroy_n(data_.GetAddress() + rhs.size_, size_ - rhs.size_);
                } else {
                    std::copy(rhs.data_.GetAddress(), rhs.data_.GetAddress() + size_, data_.GetAddress());
                    std::uninitialized_copy_n(rhs.data_.GetAddress() + size_, rhs.size_ - size_, data_.GetAddress() +size_);
                }
                size_ = rhs.size_;
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& rhs) {
        if (this != &rhs) {
            Swap(rhs);
        }
        return *this;
    }

    void Swap(Vector& other) noexcept {
        data_.Swap(other.data_);
        std::swap(size_, other.size_);
    }

    void Reserve(size_t capacity) {
        if (capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_data(capacity);
        if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
            std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
        } else {
            std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
        }
        std::destroy_n(data_.GetAddress(), size_);   
        data_.Swap(new_data);
    }

    void PopBack() noexcept {
        if (size_ != 0) {
            std::destroy_at(data_.GetAddress() + size_ - 1);
            --size_;
        }
    }
 
    void Resize(size_t new_size) {
        if (new_size < size_) {
            std::destroy_n(data_.GetAddress() + new_size, size_-new_size);
            
        } else {
            Reserve(new_size);
            std::uninitialized_value_construct_n(data_.GetAddress() + size_, new_size - size_);
        }
        size_ = new_size;
    }
    
    void PushBack(const T& value) {
        Push(value);
    }

    void PushBack(T&& value) {
        Push(std::move(value));
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

    template <typename... Args>
    iterator Emplace(const_iterator pos, Args&&... args) {
        //auto dist = std::distance(begin(), pos);
        size_t dist = pos - begin();
        T* result = nullptr;
        if (size_ = Capacity()) {
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

    template <typename... Args>
    iterator Insert(const_iterator pos, const T& value) {
        return Emplace(pos, value);
    }

    template <typename... Args>
    iterator Insert(const_iterator pos, T&& value) {
        return Emplace(pos, std::move(value));
    }

    iterator Erase(const iterator pos) {
        assert(pos >= begin() && pos < end());
        if (pos == end() - 1) {
            PopBack();
            return end();
        }
        //auto dist = std::distance(begin(), pos);
        size_t dist = pos - begin();
        std::move(begin()+dist+1, end(), begin()+dist);
        PopBack();
        return begin()+dist;
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

    iterator begin() noexcept {
        return data_.GetAddress();
    }

    iterator end() noexcept {
        return data_.GetAddress()+size_;
    }

    const_iterator begin() const noexcept {
        return data_.GetAddress();
    }

    const_iterator end() const noexcept {
        return data_.GetAddress()+size_;
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_iterator cend() const noexcept {
        return end();
    }

    ~Vector() noexcept {
        std::destroy_n(data_.GetAddress(), size_);
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;

    template <typename Tp>
    T& Push(Tp&& value) {
        if (size_ == Capacity()) {
            RawMemory<T> new_data(size_ ==0 ? 1 : size_*2);
            new(new_data + size_) T(std::forward<Tp>(value));
            if constexpr (std::is_nothrow_move_constructible_v<T> || !std::is_copy_constructible_v<T>) {
                std::uninitialized_move_n(data_.GetAddress(), size_, new_data.GetAddress());
            } else {
                std::uninitialized_copy_n(data_.GetAddress(), size_, new_data.GetAddress());
            }
            std::destroy_n(data_.GetAddress(), size_);   
            data_.Swap(new_data);
        } else {
            new (data_+size_) T(std::forward<Tp>(value));
        }
        ++size_;
        return *data_.GetAddress();
    }

};