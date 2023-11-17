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

#include <filesystem>
#include "src/document.h"
#include "src/windows/page_window.h"
#include "src/windows/turn_page_bar_win.h"

#define KEY_ENTER_ 10

using namespace std;

void checkSmallConsoleSize(short y, short x) {
    if (y < 6 || x < 48) {
        printw("Console size so small: ether y < 6 or x < 48");
        getch(); endwin(); exit(1);
    }
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    initscr(); noecho(); raw();
    keypad(stdscr, TRUE); curs_set(0); //DON'T FORGET TURN IT BACK, IF YOU NEED TO HIGHLIGHT CURSOR POSITION
    refresh(); //To display non-mutable char into the screen, which print before getch()
    short y, x; getmaxyx(stdscr, y, x);
    checkSmallConsoleSize(y, x);
    y -= 2; x -= 2;

    if (!is_regular_file(filesystem::path(argv[1]))) {
        string buff = argv[1]; buff += " isn't correct file, please put .aml file on next run";
        printw("%s", buff.c_str());
        getch(); endwin(); return 2;
    }
    Document doc(argv[1]);

    TurnPageBarWindow turnPageBarWin(x, y, doc.getPageCount());
    PageWindow pageWin(y, x, 0, doc.getText());

    string sp_buff;
    while(true) {
        switch (int c = getch()) {
            case 'Q':
            case 'q':
                endwin();
                return 0;
            case KEY_UP:
                pageWin.scrollUp();
                break;
            case KEY_DOWN:
                pageWin.scrollBack();
                break;
            case KEY_LEFT:
                if (!doc.isPrevPageNull()) {
                    doc.gotoPrevPage();
                    pageWin.changeTextAndPrint(doc.getText());
                    turnPageBarWin.decPageNumber();
                    turnPageBarWin.printCurrentPageNumber();
                }
                break;
            case KEY_RIGHT:
                if (!doc.isNextPageNull()) {
                    doc.gotoNextPage();
                    pageWin.changeTextAndPrint(doc.getText());
                    turnPageBarWin.incPageNumber();
                    turnPageBarWin.printCurrentPageNumber();
                }
                break;
            case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
                turnPageBarWin.writeDigit(c);
                break;
            case KEY_BACKSPACE:
                turnPageBarWin.eraseOneDigit();
                break;
            case KEY_ENTER_:
                if (doc.getPageCount() >= turnPageBarWin.getPageNum()
                && turnPageBarWin.getPageNum() != 0) {
                    doc.gotoPage(turnPageBarWin.getPageNum()-1);
                    pageWin.changeTextAndPrint(doc.getText());
                    turnPageBarWin.updateCurrentPageNumber();
                    turnPageBarWin.printCurrentPageNumber();
                }
                break;
            case KEY_RESIZE:
                getmaxyx(stdscr, y, x);
                checkSmallConsoleSize(y, x);
                y -= 2; x -= 2;
                clear(); refresh();
                pageWin.resize(y, x, 0, doc.getText());
                turnPageBarWin.resize(y, x, doc.getPageCount());
                break;
            default:
                break;
        }
    }
}