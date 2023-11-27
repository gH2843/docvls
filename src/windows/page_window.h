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
#ifndef P_AML_PAGE_WINDOW_H
#define P_AML_PAGE_WINDOW_H

#include "ncurses.h"
#include "../soft_wrap.h"

using namespace std;

class PageWindow {
    WINDOW* win;
    int pos;
    string whole_text;
    
    void printPage(short y, short x) {
        mvwprintw(win, 1, 0, "%s", whole_text.substr(pos * (x + 1) , y * (x + 1)).c_str());
        box(win, 0, 0);
    }
public:
    PageWindow(short y, short x, int pos, const string& text) {
        this->win = newwin(y - 6, x, 3, 0);
        this->pos = pos;
        this->whole_text = softWrap(text, x - 2);
        printPage(win->_maxy, win->_maxx);
        wrefresh(win);
    }
    ~PageWindow() {
        delwin(win);
    }

    void resize(short y, short x, int pos_, const string& text) {
        wresize(win, y - 6, x);
        werase(win);
        this->pos = pos_;
        this->whole_text = softWrap(text, x - 2);
        printPage(win->_maxy, win->_maxx);
        wnoutrefresh(win);
    }
    void resize(short y, short x) {
        wresize(win, y - 6, x);
        werase(win);
        box(win, 0, 0);
        wnoutrefresh(win);
    }

    void refresh() {
        wrefresh(win);
    }
    void noutrefresh() {
        wnoutrefresh(win);
    }

    //"_RF" mean refresh
    void scrollUp_RF() {
        if (pos != 0) {
            --pos;
            printPage(win->_maxy, win->_maxx);
            wrefresh(win);
        }
    }
    //"_RF" mean refresh
    void scrollBack_RF() {
        if (whole_text.size() / (win->_maxx) > pos + win->_maxy-1) {
            ++pos;
            printPage(win->_maxy, win->_maxx);
            wrefresh(win);
        }
    }

    void mvprintWithAttr(short y, short x, const string& text, chtype attr) {
        wattron(win, attr);
        mvwprintw(win, y, x, "%s", text.c_str());
        wattroff(win, attr);
    }
    void mvprintWithAttr(short y, short x, const char& text, chtype attr) {
        wattron(win, attr);
        mvwprintw(win, y, x, "%c", text);
        wattroff(win, attr);
    }

    //"_NORF" mean nooutrefresh()
    void changeText_NORF(const string& text) {
        whole_text = softWrap(text, win->_maxx-1);
        pos = 0;
        werase(win);
        printPage(win->_maxy, win->_maxx);
        wnoutrefresh(win);
    }
};

#endif //P_AML_PAGE_WINDOW_H