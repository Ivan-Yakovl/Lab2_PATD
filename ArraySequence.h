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

    // Внутренние методы модификации
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
        if (index < 0 || index > items->GetSize()) {
            throw std::out_of_range("Index out of range");
        }
        items->Resize(items->GetSize() + 1);
        for (int i = items->GetSize() - 1; i > index; i--) {
            items->Set(i, items->Get(i - 1));
        }
        items->Set(index, item);
    }

public:
    // Конструкторы
    ArraySequence(T* items, int count) : items(new DynamicArray<T>(items, count)) {}
    ArraySequence() : items(new DynamicArray<T>(0)) {}
    ArraySequence(const DynamicArray<T>& arr) : items(new DynamicArray<T>(arr)) {}

    virtual ~ArraySequence() {
        delete items;
    }

    // Чисто виртуальные методы
    virtual ArraySequence<T>* Instance() = 0;
    virtual ArraySequence<T>* Clone() const = 0;

    // Основной интерфейс
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

    // Методы доступа
    T GetFirst() const override {
        if (items->GetSize() == 0) throw std::out_of_range("Sequence is empty");
        return items->Get(0);
    }

    T GetLast() const override {
        if (items->GetSize() == 0) throw std::out_of_range("Sequence is empty");
        return items->Get(items->GetSize() - 1);
    }

    T Get(int index) const override {
        return items->Get(index);
    }

    int GetLength() const override {
        return items->GetSize();
    }

    // Получение подпоследовательности
    ArraySequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= items->GetSize() || startIndex > endIndex) {
            throw std::out_of_range("Invalid subsequence range");
        }

        int subSize = endIndex - startIndex + 1;
        T* subItems = new T[subSize];
        for (int i = 0; i < subSize; i++) {
            subItems[i] = items->Get(startIndex + i);
        }

        ArraySequence<T>* result = new MutableArraySequence<T>(subItems, subSize);
        delete[] subItems;
        return result;
    }

    // Конкатенация
    ArraySequence<T>* Concat(Sequence<T>* other) override {
        ArraySequence<T>* newseq = Instance();
        if (other == nullptr) throw std::invalid_argument("Other sequence is null");

        int oldSize = newseq->GetLength();
        int otherSize = other->GetLength();
        newseq->items->Resize(oldSize + otherSize);

        for (int i = 0; i < otherSize; i++) {
            newseq->items->Set(oldSize + i, other->Get(i));
        }
        return newseq;
    }
};

// Mutable версия
template <class T>
class MutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    ArraySequence<T>* Instance() override {
        return this;
    }

    ArraySequence<T>* Clone() const override {
        return new MutableArraySequence<T>(*this->items);
    }
};

// Immutable версия
template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    ArraySequence<T>* Instance() override {
        return this->Clone();
    }

    ArraySequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this->items);
    }
};

#endif