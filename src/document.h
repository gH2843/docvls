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

#pragma once //aml - simple ml for work with a presentations into a terminal.
#ifndef P_AML_DOCUMENT_H
#define P_AML_DOCUMENT_H

#include <string>
#include <fstream>
#include "doubly_linked_list.h"

using namespace std;

class Document {
    short page_count = 0;
    void addPage(string& text, string& name) {
        pages.pushBack(Page(text, name));
        ++page_count;
    }
    DoublyLinkedList pages;
    Node* page = pages.head;
public:
    explicit Document(const string& doc_path) {
        std::ifstream doc (doc_path);
        string line, raw_doc;
        raw_doc.reserve(4096);
        while (doc) {
            std::getline(doc, line);
            raw_doc += line + '\n';
        }

        string buffer, name;
        for(long unsigned int i = 1; i < raw_doc.size(); ++i) {
            if(raw_doc[i] == '*') {
                addPage(buffer, name);
                buffer = "";
            } else {
                if(raw_doc[i] == '#') {
                    name = "";
                    for(i += 1; raw_doc[i] != '\n'; ++i) {
                        name += raw_doc[i];
                    }
                    ++i;
                }
                buffer += raw_doc[i];
            }
        }
        page = pages.head;
    }
    ~Document() {
        pages.clear();
    }

    bool isPrevPageNull() {
        return page->prev == nullptr;
    }
    bool isNextPageNull() {
        return page->next == nullptr;
    }


    [[nodiscard]] short getPageCount() const {
        return page_count;
    }
    [[nodiscard]] const string& getText() const {
        return page->data.text;
    }
    [[nodiscard]] const string& getName() const {
        return page->data.name;
    }

    void gotoNextPage() {
        page = page->next;
    }
    void gotoPrevPage() {
        page = page->prev;
    }
    void gotoPage(int number) {
        page = pages[number];
    }
};

#endif //P_AML_DOCUMENT_H