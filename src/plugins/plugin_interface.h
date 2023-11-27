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

#include "../windows/page_window.h"
#include "../windows/turn_page_bar_win.h"

class PluginInterface {
protected:
    PageWindow* pageWin;
    TurnPageBarWindow* turnPageBarWin;
    pair<WINDOW*, string> rawTopBarWin_with_content;
public:
    PluginInterface(PageWindow* pageWin, TurnPageBarWindow* turnPageBarWin,
                    WINDOW* rawTopBarWin) {
        this->pageWin = pageWin;
        this->turnPageBarWin = turnPageBarWin;
        this->rawTopBarWin_with_content = pair(rawTopBarWin, "need to update on each call topTapLoop");
    }

    virtual void topTabLoop(const pair<vector<string>, int>& buffer_topBarWin) = 0;

    void resize(const string& text_for_PageWin, const string& active_plugin, int active_plugin_pos) {
        short y, x;
        getmaxyx(stdscr, y, x);

        WINDOW* rawTopBarWin = rawTopBarWin_with_content.first;
        wresize(rawTopBarWin, 3, x);
        werase(rawTopBarWin);
        mvwprintw(rawTopBarWin, 1, 1, "%s", rawTopBarWin_with_content.second.c_str());
        wattron(rawTopBarWin, WA_REVERSE);
        mvwprintw(rawTopBarWin, 1, active_plugin_pos, "%s", active_plugin.c_str());
        wattroff(rawTopBarWin, WA_REVERSE);
        mvwprintw(rawTopBarWin, 1, active_plugin_pos + active_plugin.size()-3, "%s", "│");
        box(rawTopBarWin, 0, 0);
        wnoutrefresh(rawTopBarWin);

        turnPageBarWin->resize(y, x, 0);
        pageWin->resize(y, x, 0, text_for_PageWin);
        doupdate();
    }

    virtual ~PluginInterface() = default;
};

#endif //DOCVLS_PLUGIN_INTERFACE_H
