#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdexcept>

template <class T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data) : data(data), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    int size;

public:
   
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(T* items, int count) : LinkedList() {
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& list) : LinkedList() {
        Node* current = list.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    
    LinkedList& operator=(const LinkedList<T>& list) {
        if (this != &list) {
           
            while (head != nullptr) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            size = 0;

            
            Node* current = list.head;
            while (current != nullptr) {
                Append(current->data);
                current = current->next;
            }
        }
        return *this;
    }

    
    T GetFirst() const {
        if (size == 0) {
            throw std::out_of_range("List is empty");
        }
        return head->data;
    }

    T GetLast() const {
        if (size == 0) {
            throw std::out_of_range("List is empty");
        }
        return tail->data;
    }

    T Get(int index) const {
        if (index < 0 || index >= size) {
            throw std::out_of_range("Index out of range");
        }

        Node* current = head;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->data;
    }

    int GetSize() const {
        return size;
    }

    
    void Append(T item) {
        Node* newNode = new Node(item);
        if (tail == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void Prepend(T item) {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            newNode->next = head;
            head = newNode;
        }
        size++;
    }

    void InsertAt(T item, int index) {
        if (index < 0 || index > size) {
            throw std::out_of_range("Index out of range");
        }

        if (index == 0) {
            Prepend(item);
        }
        else if (index == size) {
            Append(item);
        }
        else {
            Node* newNode = new Node(item);
            Node* prev = head;
            for (int i = 0; i < index - 1; ++i) {
                prev = prev->next;
            }
            newNode->next = prev->next;
            prev->next = newNode;
            size++;
        }
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
            throw std::out_of_range("Invalid index range");
        }

        LinkedList<T>* subList = new LinkedList<T>();
        Node* current = head;
        for (int i = 0; i < startIndex; ++i) {
            current = current->next;
        }
        for (int i = startIndex; i <= endIndex; ++i) {
            subList->Append(current->data);
            current = current->next;
        }
        return subList;
    }

    LinkedList<T>* Concat(LinkedList<T>* list) const {
        if (list == nullptr) {
            throw std::invalid_argument("List cannot be null");
        }

        LinkedList<T>* newList = new LinkedList<T>(*this);
        Node* current = list->head;
        while (current != nullptr) {
            newList->Append(current->data);
            current = current->next;
        }
        return newList;
    }
};

#endif 