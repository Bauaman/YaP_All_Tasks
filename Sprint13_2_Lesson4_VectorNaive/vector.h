#pragma once
#include <cassert>
#include <cstdlib>
#include <new>
#include <utility>

template <typename T>
class Vector {
public:
    Vector() = default;

    explicit Vector(size_t size) :
        data_(Allocate(size)),
        capacity_(size),
        size_(size)
    {
        for (size_t i = 0; i!=size; ++i) {
            new(data_+i) T();
        }
    }

    Vector(const Vector& other) :
        data_(Allocate(other.Size())),
        capacity_(other.Size()),
        size_(other.Size())
        {
            for (size_t i = 0; i!= size_; ++i) {
                CopyConstruct(data_+i, other.data_[i]);
            }
        }

    void Reserve(size_t capacity) {
        if (capacity <= capacity_) {
            return;
        }
        T* new_data = Allocate(capacity);
        for (size_t i = 0; i != size_; ++i) {
            CopyConstruct(new_data + i, data_[i]);
        }
        DestroyN(data_, size_);
        Deallocate(data_);
        data_ = new_data;
        capacity_ = capacity;
    }
 
    size_t Size() const noexcept {
        return size_;
    }

    size_t Capacity() const noexcept {
        return capacity_;
    }

    const T& operator[](size_t index) const noexcept {
        return const_cast<Vector&>(*this)[index];
    }

    T& operator[](size_t index) noexcept {
        assert(index < size_);
        return data_[index];
    }

    ~Vector() noexcept {
        DestroyN(data_, size_);
        Deallocate(data_);
    }

private:
    T* data_ = nullptr;
    size_t capacity_ = 0;
    size_t size_ = 0;

    static T* Allocate(size_t size) {
        return size!=0 ? static_cast<T*>(operator new(size* sizeof(T))) : nullptr;
    }

    static void CopyConstruct(T* buf, const T& elem) {
        new(buf) T(elem);
    }

    static void Deallocate(T* buf) noexcept {
        operator delete(buf);
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