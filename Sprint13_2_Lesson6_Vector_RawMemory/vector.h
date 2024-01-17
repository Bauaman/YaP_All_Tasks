#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>

template <typename T>
class RawMemory {
public:
    RawMemory() = default;

    RawMemory(size_t cap) :
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
    Vector() = default;

    explicit Vector(size_t size) :
        data_(size),
        size_(size)
    {
        size_t i = 0;
        try {
            for (; i!=size; ++i) {
                new(data_+i) T();
            }
        } catch (...) {
            DestroyN(data_.GetAddress(), i);
            throw;
        }
    }

    Vector(const Vector& other) :
        data_(other.Size()),
        size_(other.Size())
        {
            size_t i=0;
            try {
                for (; i!= size_; ++i) {
                    CopyConstruct(data_+i, other.data_[i]);
                }
            } catch (...) {
                DestroyN(data_.GetAddress(), i);
                throw;
            }
        }

    void Reserve(size_t capacity) {
        if (capacity <= data_.Capacity()) {
            return;
        }
        RawMemory<T> new_data(capacity);
        size_t i = 0;
        try {
            for (; i != size_; ++i) {
                CopyConstruct(new_data + i, data_[i]);
            }
        } catch (...) {
            DestroyN(new_data.GetAddress(),i);

            throw;
        }
        DestroyN(data_.GetAddress(), size_);
        data_.Swap(new_data);
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

    ~Vector() noexcept {
        DestroyN(data_.GetAddress(), size_);
    }

private:
    RawMemory<T> data_;
    size_t size_ = 0;

    static void CopyConstruct(T* buf, const T& elem) {
        new(buf) T(elem);
    }

    static void DestroyN(T* buf, size_t size) {
        for (size_t i = 0; i != size; ++i) {
            Destroy(buf + i);
        }
    }

    static void Destroy(T* buf) {
        buf->~T();
    }


};