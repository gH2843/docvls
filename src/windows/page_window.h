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

#ifndef P_AML_PAGE_WINDOW_H
#define P_AML_PAGE_WINDOW_H
#pragma once

#include <string>
#include "ncurses.h"
#include "../soft_wrap.h"

using namespace std;

class PageWindow {
    WINDOW* win;
    int pos;
    string whole_text;
    string current_text;

    void printPage(short y) {
        int n_count = 0, i = pos;
        current_text.clear();
        for (; i < whole_text.size() && n_count != y; ++i) {
            if (whole_text[i] == '\n') {
                ++n_count;
            }
            current_text += whole_text[i];
        }
        if (current_text[current_text.size()-1] == '\n') {
            current_text.pop_back();
        }

        wclear(win);
        mvwprintw(win, 1, 1, "%s", current_text.c_str());
        box(win, 0, 0); //take out
        wrefresh(win);
    }
public:
    PageWindow(short y, short x, int pos, const string& text) {
        this->win = newwin(y - 1, x + 2, 0, 0);
        this->pos = pos;
        this->whole_text = softWrap(text, x);

        printPage(win->_maxy + 1);
    }
    void resize(short y, short x, int pos_, const string& text) {
        win = newwin(y - 1, x + 2, 0, 0);
        this->pos = pos_;
        this->whole_text = softWrap(text, x);

        printPage(win->_maxy + 1);
    }
    void resize(short y, short x) {
        win = newwin(y - 1, x + 2, 0, 0);

        wclear(win);
        box(win, 0, 0); // take out
        wrefresh(win);
    }
    void scrollUp() {
        if (pos > 0) {
            pos -= 3;
            for (; pos > 0 && whole_text[pos] != '\n'; --pos) {}
            if(pos > 0) { pos += 2; }
            printPage(win->_maxy + 1);
        }
    }
    void scrollBack() { //todo: add protection against scroll so far
        for (; pos < whole_text.size() && whole_text[pos] != '\n'; ++pos) { }
        pos += 2;
        printPage(win->_maxy + 1);
    }

    void changeTextAndPrint(const string& text) {
        whole_text = softWrap(text, win->_maxx-1);
        pos = 0;
        printPage(win->_maxy + 1);
    }
};

#endif //P_AML_PAGE_WINDOW_H