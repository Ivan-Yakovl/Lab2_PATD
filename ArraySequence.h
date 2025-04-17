#ifndef ARRAY_SEQUENCE_H
#define ARRAY_SEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"

template <class T> class MutableArraySequence;
template <class T> class ImmutableArraySequence;

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* items;

    
    void AppendImpl(T item) {
        items->Resize(items->GetSize() + 1);
        items->Set(items->GetSize() - 1, item);
    }

    void PrependImpl(T item) {
        items->Resize(items->GetSize() + 1);
        for (int i = items->GetSize() - 1; i > 0; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(0, item);
    }

    void InsertAtImpl(T item, int index) {
       
        items->Resize(items->GetSize() + 1);
        for (int i = items->GetSize() - 1; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
    }

public:
    
    ArraySequence(T* items, int count)
        : items(new DynamicArray<T>(items, count)) {}

    // Конструктор по умолчанию
    ArraySequence()
        : items(new DynamicArray<T>(0)) {}

    // Конструктор из DynamicArray
    ArraySequence(const DynamicArray<T>& arr)
        : items(new DynamicArray<T>(arr)) {}

    // Конструктор копирования
    ArraySequence(const ArraySequence& other)
        : items(new DynamicArray<T>(*other.items)) {}


    ArraySequence& operator=(const ArraySequence& other) {
        if (this != &other) {
            delete items;
            items = new DynamicArray<T>(*other.items);
        }
        return *this;
    }

    virtual ~ArraySequence() {
        delete items;
    }

   
    virtual ArraySequence<T>* Instance() = 0;
    virtual ArraySequence<T>* Clone() const = 0;

    
    ArraySequence<T>* Append(T item) override {
        ArraySequence<T>* newseq = Instance();
        newseq->AppendImpl(item);
        return newseq;
    }

    ArraySequence<T>* Prepend(T item) override {
        ArraySequence<T>* newseq = Instance();
        newseq->PrependImpl(item);
        return newseq;
    }

    ArraySequence<T>* InsertAt(T item, int index) override {
        ArraySequence<T>* newseq = Instance();
        newseq->InsertAtImpl(item, index);
        return newseq;
    }

    
    T GetFirst() const override {
        
        return items->Get(0);
    }

    T GetLast() const override {
        
        return items->Get(items->GetSize() - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    
    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
       

        int subSize = endIndex - startIndex + 1;
        T* subItems = new T[subSize];
        for (int i = 0; i < subSize; i++) {
            subItems[i] = items->Get(startIndex + i);
        }

        ArraySequence<T>* result = new MutableArraySequence<T>(subItems, subSize);
        delete[] subItems;
        return result;
    }

   
    ArraySequence<T>* Concat(Sequence<T>* other) override {
        ArraySequence<T>* newseq = Instance();
       

        int oldSize = newseq->GetLength();
        int otherSize = other->GetLength();
        newseq->items->Resize(oldSize + otherSize);

        for (int i = 0; i < otherSize; i++) {
            newseq->items->Set(oldSize + i, other->Get(i));
        }
        return newseq;
    }
};

template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    // Явно объявляем конструкторы
    MutableArraySequence() : ArraySequence<T>() {}

    MutableArraySequence(T* items, int count)
        : ArraySequence<T>(items, count) {}

    MutableArraySequence(const DynamicArray<T>& arr)
        : ArraySequence<T>(arr) {}

    // Конструктор копирования
    MutableArraySequence(const MutableArraySequence& other)
        : ArraySequence<T>(other) {}  // Используем конструктор копирования базового класса

    ArraySequence<T>* Instance() override {
        return this;
    }

    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this);
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence(T* items, int count)
        : ArraySequence<T>(items, count) {}

    ImmutableArraySequence()
        : ArraySequence<T>() {}

    ImmutableArraySequence(const DynamicArray<T>& arr)
        : ArraySequence<T>(arr) {}


    ArraySequence<T>* Instance() override {
        return this->Clone();
    }

    ArraySequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this->items);
    }
};

#endif