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
#ifndef DOCVLS_PLUGIN_MANAGER_H
#define DOCVLS_PLUGIN_MANAGER_H

#include <filesystem>
#include <vector>
#include <set>
#include <dlfcn.h>
#include "../windows/top_bar_window.h"
#include "plugin_interface.h"

#define KEY_ENTER_ 10
#define PLUGIN_MANAGER_KEY KEY_F(1)

namespace fs = filesystem;

class PluginManager: public PluginInterface{ //todo: merge with top_bar_win
    fs::path path_to_plugins;
    set<string> active_plugins;
    unordered_map<string, pair<int, void*>> dynamicLibraries;
    TopBarWindow* topBarWin;
public:
    PluginManager(PageWindow* pageWin, TurnPageBarWindow* turnPageBarWin,TopBarWindow* topBarWin)
                 : PluginInterface(pageWin, turnPageBarWin, topBarWin->getWin()) {
        this->path_to_plugins = (string(getenv("HOME")) + "/.config/docvls/plugins");
        this->topBarWin = topBarWin;
    }

    ~PluginManager() override {
        for (auto& library : dynamicLibraries) {
            dlclose(library.second.second);
        }
    }

    void topTabLoop(const pair<vector<string>, int>& buffer_topBarWin) override {
        rawTopBarWin_with_content.second = buffer_topBarWin.first[1];
        vector<string> plugins;

        topBarWin->printTabsWithSelectOne(PLUGIN_MANAGER_KEY);
        topBarWin->refresh();

        short pos = 1;
        for(auto const& file: fs::directory_iterator(path_to_plugins)) {
            string plugin_name = file.path().filename().string() + " [ ]";
            if (active_plugins.find(plugin_name) != active_plugins.end()) {
                plugin_name[plugin_name.size() - 2] = '*';
            }
            plugins.push_back(plugin_name);

            pageWin->mvprintWithAttr(pos, 1, plugins[pos-1], WA_NORMAL);
            ++pos;
        }
        pos = 0; // "select" first plugin
        pageWin->mvprintWithAttr(1, 1, plugins[0], WA_REVERSE);
        pageWin->refresh();

        WINDOW* rawTopBarWin = rawTopBarWin_with_content.first;
        keypad(rawTopBarWin, TRUE);
        while (true) {
            switch (wgetch(rawTopBarWin)) {
                case KEY_RESIZE: {
                    string buff, active_plugin;
                    short y = 0, active_plugin_y = 0;
                    for (const auto &i: plugins) {
                        if( y == pos) {
                            active_plugin = i;
                            active_plugin_y = y + 1;
                        }
                        ++y;
                        buff += i + "\n";
                    }

                    resize(buff, buffer_topBarWin.first[0], buffer_topBarWin.second);
                    topBarWin->printTabsWithSelectOne(PLUGIN_MANAGER_KEY);
                    topBarWin->noutrefresh();
                    pageWin->mvprintWithAttr(active_plugin_y, 1, active_plugin, WA_REVERSE);
                    pageWin->noutrefresh();
                    doupdate();
                    break;
                }
                case 'q':
                    goto escape;
                case KEY_DOWN:
                    if (pos < plugins.size()-1) {
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_NORMAL);
                        ++pos;
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_REVERSE);
                        pageWin->refresh();
                    }
                    break;
                case KEY_UP:
                    if (pos > 0) {
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_NORMAL);
                        --pos;
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_REVERSE);
                        pageWin->refresh();
                    }
                    break;
                case KEY_ENTER_: // if "<plugin_name> [ ]" not contains '*' - plugin not loaded.
                    if (plugins[pos][plugins[pos].size()-2] == ' ') {
                        active_plugins.emplace(plugins[pos]);
                        plugins[pos][plugins[pos].size()-2] = '*';
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_REVERSE);
                        pageWin->refresh();

                        string path = path_to_plugins.string()+"/"+plugins[pos].substr(0, plugins[pos].length()-4);
                        void* library = dlopen(path.c_str(), RTLD_NOW);
                        if (library != nullptr) {
                            dynamicLibraries.emplace(plugins[pos], pair(0, library));

                            typedef PluginInterface* (*Init)(PageWindow*, TurnPageBarWindow*, WINDOW*, int&);
                            Init init = (Init)dlsym(library, "getPluginClass");
                            if (init != nullptr) {
                                int key;
                                PluginInterface* pPlugin = init(pageWin, turnPageBarWin, topBarWin->getWin(), key);
                                dynamicLibraries[plugins[pos]].first = key;

                                topBarWin->createTab(key, plugins[pos].substr(0, plugins[pos].length()-4), pPlugin);
                                topBarWin->printTabsWithSelectOne(PLUGIN_MANAGER_KEY);
                                topBarWin->refresh();
                            } else {
                                dlclose(library);
                            }
                        } else {
                            exit(1); // todo: print error message
                        }
                    } else {
                        topBarWin->deleteTab_NORF(dynamicLibraries[plugins[pos]].first);

                        dlclose(dynamicLibraries[plugins[pos]].second);
                        dynamicLibraries.erase(plugins[pos]);

                        plugins[pos][plugins[pos].size()-2] = ' ';
                        active_plugins.erase(plugins[pos]);
                        pageWin->mvprintWithAttr(pos+1, 1, plugins[pos], WA_REVERSE);
                        pageWin->noutrefresh();

                        doupdate();
                    }
                    break;
            }
        }
        escape:
    }
};

#endif //DOCVLS_PLUGIN_MANAGER_H
