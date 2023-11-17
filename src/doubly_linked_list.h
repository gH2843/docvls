/* docvls - document viewer with less and soft-wrap functionality.
Copyright (C) 2023  gH2843

This file is part of docvls.

docvls is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>. */

#ifndef P_AML_DOUBLY_LINKED_LIST_H
#define P_AML_DOUBLY_LINKED_LIST_H
#pragma once

#include <utility>

using namespace std;
string q, w;

class Page {
    string name, text;
public:
    Page(string& text, string& name) {
        this->name = name;
        this->text = text;
    }
    friend class Document;
};

class Node {
    Node* next;
    Node* prev;
    Page data = Page(q, w);
public:
    explicit Node(Page data, Node* next = nullptr, Node* prev = nullptr) {
        this->data = std::move(data);
        this->next = next;
        this->prev = prev;
    }
    friend class TurnPageBarWindow;
    friend class DoublyLinkedList;
    friend class Document;
};

class DoublyLinkedList {
    int size;
public:
    Node* head;
    Node* tail;

    DoublyLinkedList() {
        size = 0;
        head = nullptr;
        tail = nullptr;
    }
    ~DoublyLinkedList() {
        clear();
    } //todo: Check how the destructor works. Maybe there is memory leak!

    void pushBack(const Page& data) {
        if (head == nullptr) {
            head = new Node(data);
            tail = head;
        } else {
            auto temp = tail;
            tail = new Node(data, nullptr, temp);
            temp->next = tail;
        }
        ++size;
    }
    void popFront() {
        Node* temp = head;
        head = head->next;
        delete temp;
        --size;
    }

    void clear() {
        while (size) {
            popFront();
        }
    }

    [[nodiscard]] int getSize() const { return size; }

    Node* operator[](const int index) const {
        int counter = 0;
        Node* current = this->head;
        while (current != nullptr) {
            if (counter == index) {
                return current;
            }
            current = current->next;
            ++counter;
        }
        return nullptr;
    }
};

#endif //P_AML_DOUBLY_LINKED_LIST_H