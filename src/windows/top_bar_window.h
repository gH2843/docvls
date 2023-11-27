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
#ifndef DOCVLS_TOP_BAR_WINDOW_H
#define DOCVLS_TOP_BAR_WINDOW_H

#include <vector>
#include <unordered_map>
#include "../plugins/plugin_interface.h"

using namespace std;

class TopBarWindow{
    WINDOW* win;
    unordered_map<int, PluginInterface*> plugins;
    unordered_map<int, string> tabs;

    pair<vector<string>, int> getPluginsAsString(const int& key) {
        vector<string> plugins_string;
        plugins_string.emplace_back("");
        plugins_string.emplace_back("");

        short pos = 1, active_plugin_pos;
        for (const auto& tab : tabs) {
            if (tab.first == key) {
                plugins_string[0] = tab.second + "│";
                active_plugin_pos = pos;
            }
            plugins_string[1] += tab.second + "│";
            pos += tab.second.size() + 1;
        }

        return pair(plugins_string, active_plugin_pos);
    }
public:
    TopBarWindow(short x) {
        this->win = newwin(3, x, 0, 0);
        box(win, 0, 0);
        wrefresh(win);
    }
    ~TopBarWindow() {
        delwin(win);
        for (auto& plugin : plugins) {
            delete plugin.second;
        }
    }

    void resize(short x) {
        wresize(win, 3, x);
        printTabs();
        wnoutrefresh(win);
    }

    void refresh() {
        wrefresh(win);
    }
    void noutrefresh() {
        wnoutrefresh(win);
    }

    void printTabs() {
        werase(win);
        short pos = 1;
        for (const auto& tab : tabs) {
            mvwprintw(win, 1, pos, "%s", (tab.second + "│").c_str());
            pos += tab.second.size() + 1;
        }
        box(win, 0, 0);
    }
    void printTabsWithSelectOne(const int& key) {
        werase(win);
        short pos = 1;
        for (const auto& tab : tabs) {
            if (tab.first == key) {
                wattron(win, WA_REVERSE);
                mvwprintw(win, 1, pos, "%s", tab.second.c_str());
                wattroff(win, WA_REVERSE);
                wprintw(win, "%s", "│");
            } else {
                mvwprintw(win, 1, pos, "%s", (tab.second + "│").c_str());
            }
            pos += tab.second.size() + 1;
        }
        box(win, 0, 0);
    }

    void createTab(const int& key, const string& name, PluginInterface* plugin) {
        plugins.emplace(key, plugin);
        tabs.emplace(key, name);
        printTabs();
    }
    void openTab(const int& key) {
        plugins[key]->topTabLoop(getPluginsAsString(key));
    }
    //"_NORF" mean noutrefresh()
    void deleteTab_NORF(const int& key) {
        delete plugins[key];
        plugins.erase(key);
        tabs.erase(key);
        printTabsWithSelectOne(key);
        wnoutrefresh(win);
    }
    bool isTab(const int& key) {
        return plugins.find(key) != plugins.end();
    }

    WINDOW* getWin() {
        return win;
    }
};

#endif //DOCVLS_TOP_BAR_WINDOW_H
