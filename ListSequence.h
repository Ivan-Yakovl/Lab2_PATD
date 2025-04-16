#ifndef LISTSEQUENCE_H
#define LISTSEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"

// Forward declarations
template <class T> class MutableListSequence;
template <class T> class ImmutableListSequence;

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;

    virtual ListSequence<T>* AppendInternal(T item) {
        list->Append(item);
        return this;
    }

    virtual ListSequence<T>* PrependInternal(T item) {
        list->Prepend(item);
        return this;
    }

    virtual ListSequence<T>* InsertAtInternal(T item, int index) {
        list->InsertAt(item, index);
        return this;
    }

    virtual ListSequence<T>* ConcatInternal(Sequence<T>* other) {
        ListSequence<T>* otherList = dynamic_cast<ListSequence<T>*>(other);
        if (!otherList) throw std::invalid_argument("Incompatible sequence type");


        LinkedList<T>* newList = new LinkedList<T>(*this->list);

        
        newList->Concat(otherList->list);

        ListSequence<T>* result = new MutableListSequence<T>(*newList);
        delete newList;     
        return result;
    }
public:
    ListSequence(T* items, int count) : list(new LinkedList<T>(items, count)) {}
    ListSequence() : list(new LinkedList<T>()) {}
    ListSequence(const LinkedList<T>& linkedList) : list(new LinkedList<T>(linkedList)) {}

    virtual ~ListSequence() {
        delete list;
    }

    virtual ListSequence<T>* Instance() = 0;
    virtual ListSequence<T>* Clone() const = 0;

    T GetFirst() const override {
        return list->GetFirst();
    }

    T GetLast() const override {
        return list->GetLast();
    }

    T Get(int index) const override {
        return list->Get(index);
    }

    int GetLength() const override {
        return list->GetSize();
    }

    ListSequence<T>* Append(T item) override {
        return Instance()->AppendInternal(item);
    }

    ListSequence<T>* Prepend(T item) override {
        return Instance()->PrependInternal(item);
    }

    ListSequence<T>* InsertAt(T item, int index) override {
        return Instance()->InsertAtInternal(item, index);
    }

    ListSequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
                LinkedList<T>* subList = list->GetSubList(startIndex, endIndex);

      
        ListSequence<T>* result = new MutableListSequence<T>(*subList);

        
        delete subList;
        return result;
    }
    ListSequence<T>* Concat(Sequence<T>* other) override {
        return Instance()->ConcatInternal(other);
    }
};

template <class T>
class MutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    ListSequence<T>* Instance() override {
        return this;
    }

    ListSequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this->list);
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;
    
    ListSequence<T>* Instance() override {
        return this->Clone();
    }

    ListSequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this->list);
    }
};

#endif