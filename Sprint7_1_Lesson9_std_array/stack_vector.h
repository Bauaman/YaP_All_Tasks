#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t N>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0)
        :array_size_(a_size)
    {
        if (a_size > N) {
            throw std::invalid_argument("Error: capacity exceeded");
        }
    }

    T& operator[](size_t index){
        return stack_vector_.at(index);
    }

    const T& operator[](size_t index) const {
        return stack_vector_.at(index);
    }

    T* begin() {
        return stack_vector_.begin();
    }

    T* end() {
        return stack_vector_.begin()+array_size_;
    }

    const T* begin() const{
        return stack_vector_.begin();
    }
    
    const T* end() const{
        return stack_vector_.begin()+array_size_;
    }

    size_t Size() const {
        return array_size_;
    }

    size_t Capacity() const {
        return N;
    }

    void PushBack(const T& value) {
        if (array_size_ == N) {
            throw std::overflow_error("Error: capacity is equal to the maximum size");
        }
        stack_vector_.at(array_size_) = value;
        ++array_size_;
    }

    T PopBack() {
        if (array_size_==0) {
            throw std::underflow_error("Error: vector is empty");
        }
        T value = stack_vector_.at(array_size_-1);
        --array_size_;
        return value;
    }

private:
    size_t array_size_;
    std::array<T, N> stack_vector_;

};
