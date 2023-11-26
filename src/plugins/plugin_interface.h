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
#ifndef DOCVLS_PLUGIN_INTERFACE_H
#define DOCVLS_PLUGIN_INTERFACE_H

#include <ncurses.h>
#include "../windows/page_window.h"
#include "../windows/turn_page_bar_win.h"

class PluginInterface {
protected:
    PageWindow* pageWin;
    TurnPageBarWindow* turnPageBarWin;
    pair<WINDOW*, string> rawTopBarWin;
public:
    PluginInterface(PageWindow* pageWin, TurnPageBarWindow* turnPageBarWin,
                    WINDOW* rawTopBarWin) {
        this->pageWin = pageWin;
        this->turnPageBarWin = turnPageBarWin;
        this->rawTopBarWin = pair(rawTopBarWin, "need to update on each call topTapLoop");
    }

    virtual void topTabLoop(const pair<vector<string>, int>& buffer_topBarWin) = 0;

    void resize(const string& text_for_PageWin, const string& active_plugin, int active_plugin_pos) {
        short y, x;
        getmaxyx(stdscr, y, x);
        clear(); refresh();

        rawTopBarWin.first = newwin(3, x, 0, 0);
        mvwprintw(rawTopBarWin.first, 1, 1, "%s", rawTopBarWin.second.c_str());
        wattron(rawTopBarWin.first, WA_REVERSE);
        mvwprintw(rawTopBarWin.first, 1, active_plugin_pos, "%s", active_plugin.c_str());
        wattroff(rawTopBarWin.first, WA_REVERSE);
        mvwprintw(rawTopBarWin.first, 1, active_plugin_pos + active_plugin.size()-3, "%s", "│");
        box(rawTopBarWin.first, 0, 0);
        wrefresh(rawTopBarWin.first);

        turnPageBarWin->resize(y, x, 0);

        pageWin->resize(y, x, 0, text_for_PageWin);
    }

    virtual ~PluginInterface() = default;
};

#endif //DOCVLS_PLUGIN_INTERFACE_H
