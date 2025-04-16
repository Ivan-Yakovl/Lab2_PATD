#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include <stdexcept>

template <class T>
class DynamicArray {
private:
    T* buffer;
    int size;

public:
    DynamicArray(T* items, int count) : size(count) {
        buffer = new T[size];
        for (int i = 0; i < size; ++i) {
            buffer[i] = items[i];
        }
    }

    DynamicArray(int size) : size(size) {
        buffer = new T[size];
    }

    DynamicArray(const DynamicArray<T>& other) : size(other.size) {
        buffer = new T[size];
        for (int i = 0; i < size; ++i) {
            buffer[i] = other.buffer[i];
        }
    }

    ~DynamicArray() {
        delete[] buffer;
    }

    DynamicArray& operator=(const DynamicArray<T>& other) {
        if (this != &other) {
            delete[] buffer;
            size = other.size;
            buffer = new T[size];
            for (int i = 0; i < size; ++i) {
                buffer[i] = other.buffer[i];
            }
        }
        return *this;
    }

    T Get(int index) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return buffer[index];
    }

    int GetSize() const {
        return size;
    }

    void Set(int index, T value) {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }
        buffer[index] = value;
    }

    void Resize(int newSize) {
        if (newSize < 0) {
            throw std::invalid_argument("Invalid size");
        }

        if (newSize == size) {
            return;
        }

        T* newBuffer = new T[newSize];
        int elementsToCopy = (size < newSize) ? size : newSize;

        for (int i = 0; i < elementsToCopy; ++i) {
            newBuffer[i] = buffer[i];
        }

        delete[] buffer;
        buffer = newBuffer;
        size = newSize;
    }
};
#endif