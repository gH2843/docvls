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
    short c_page_num; //current page number
    string sp_buff; //buffer for "select page" button;

    void printPageNum() {
        if (c_page_num < 10) {
            mvwprintw(win, 1, 2, "%s", ("0" + to_string(c_page_num)).c_str());
        } else { mvwprintw(win, 1, 2, "%d", c_page_num); }
    }

    void printSPBuffer() {
        mvwprintw(win, 1, 10, "  ");
        mvwprintw(win, 1, 10, "%s", sp_buff.c_str());
    }
public:
    TurnPageBarWindow(short x, short y, short pages_count) {
        this->win = newwin(3, x, y - 3, 0);
        this->c_page_num = 1;
        mvwprintw(win, 1, 1, "%s", ("<  >  " + to_string(pages_count) + "│[  ]│").c_str());
        printPageNum();
        box(win, 0, 0);
        mvwprintw(win, 0, 8, "┬");
        mvwprintw(win, 2, 8, "┴");
        mvwprintw(win, 0, 13, "┬");
        mvwprintw(win, 2, 13, "┴");
        wrefresh(win);
    }
    ~TurnPageBarWindow() {
        delwin(win);
    }

    void resize(short y, short x, short pages_count) {
        delwin(win);
        win = newwin(3, x, y - 3, 0);
        mvwprintw(win, 1, 1, "%s", ("<  >  " + to_string(pages_count) + "│[  ]│").c_str());
        printPageNum();
        printSPBuffer();
        box(win, 0, 0);
        mvwprintw(win, 0, 8, "┬");
        mvwprintw(win, 2, 8, "┴");
        mvwprintw(win, 0, 13, "┬");
        mvwprintw(win, 2, 13, "┴");
        wnoutrefresh(win);
    }

    //"_NORF" mean nououtrefresh()
    void incPageNum_NORF() {
        ++c_page_num;
        printPageNum();
        wnoutrefresh(win);
    }
    //"_NORF" mean nououtrefresh()
    void decPageNum_NORF() {
        --c_page_num;
        printPageNum();
        wnoutrefresh(win);
    }

    //"_NORF" mean nououtrefresh()
    void updatePageNumWithPageNumToGo_NORF() {
        c_page_num = static_cast<short>(stoi(sp_buff));
        printPageNum();
        wnoutrefresh(win);
    }

    //"_RF" mean refresh
    void writeDigitToPageNumToGo_RF(int c) {
        if (sp_buff.size() < 2) {
            sp_buff += (char) c;
            printSPBuffer();
            wrefresh(win);
        }
    }
    //"RF"mean refresh
    void eraseDigitFromPageNumToGo_RF() {
        if (!sp_buff.empty()) {
            sp_buff.pop_back();
            printSPBuffer();
            wrefresh(win);
        }
    }
    short getPageNumToGo() {
        return static_cast<short>(stoi(sp_buff));
    }
};

#endif //P_AML_TURN_PAGE_BAR_WIN_H
