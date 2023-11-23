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

#pragma once
#ifndef P_AML_TURN_PAGE_BAR_WIN_H
#define P_AML_TURN_PAGE_BAR_WIN_H

#include <ncurses.h>

using namespace std;

class TurnPageBarWindow {
    WINDOW* win;
    short c_page; //current page
    string sp_buff; //buffer for "select page" button;

    void printSPBuffer() {
        mvwprintw(win, 1, 10, "  ");
        mvwprintw(win, 1, 10, "%s", sp_buff.c_str());
        wrefresh(win);
    }
public:
    void printCurrentPageNumber() {
        wmove(win, 1, 2);
        if (c_page < 10) {
            wprintw(win, "%s", ("0" + to_string(c_page)).c_str());
        } else { wprintw(win, "%d", c_page); }
        wrefresh(win);
    }
    TurnPageBarWindow(short x, short y, short pages_count) {
        this->win = newwin(3, x + 2, y - 1, 0);
        this->c_page = 1;
        mvwprintw(win, 1, 1, "%s", ("<  >  " + to_string(pages_count) + "│[  ]│").c_str());
        printCurrentPageNumber();
        box(win, 0, 0);
        mvwprintw(win, 0, 8, "┬");
        mvwprintw(win, 2, 8, "┴");
        mvwprintw(win, 0, 13, "┬");
        mvwprintw(win, 2, 13, "┴");
        wrefresh(win);
    }
    void resize(short y, short x, short pages_count) {
        win = newwin(3, x + 2, y - 1, 0);
        mvwprintw(win, 1, 1, "%s", ("<  >  " + to_string(pages_count) + "│[  ]│").c_str());
        printCurrentPageNumber();
        printSPBuffer();
        box(win, 0, 0);
        mvwprintw(win, 0, 8, "┬");
        mvwprintw(win, 2, 8, "┴");
        mvwprintw(win, 0, 13, "┬");
        mvwprintw(win, 2, 13, "┴");
        wrefresh(win);
    }

    void incPageNumber() {
        ++c_page;
    }
    void decPageNumber() {
        --c_page;
    }

    void writeDigit(int c) {
        if (sp_buff.size() < 2) {
            sp_buff += (char) c;
            printSPBuffer();
        }
    }
    void eraseOneDigit() {
        if (!sp_buff.empty()) {
            sp_buff.pop_back();
            printSPBuffer();
        }
    }
    void updateCurrentPageNumber() {
        c_page = static_cast<short>(stoi(sp_buff));
    }
    short getPageNum() {
        return static_cast<short>(stoi(sp_buff));
    }
};

#endif //P_AML_TURN_PAGE_BAR_WIN_H
