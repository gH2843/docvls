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

#include <string>
#include <vector>
#include "../src/plugins/plugin_interface.h"
#define PLUGIN_KEY KEY_F(2)
#define KEY_ENTER_ 10
#define OFFSET 2

using namespace std;

class russDivideMethod: public PluginInterface{
    string expression, buffer;
    bool is_star;
public:
    russDivideMethod(PageWindow* pageWin, TurnPageBarWindow* turnPageBarWin, WINDOW* rawTopBarWin):
            PluginInterface(pageWin, turnPageBarWin, rawTopBarWin) { is_star = false; }

    void topTabLoop(const pair<vector<string>, int>& buffer_topBarWin) override {
        rawTopBarWin.second = buffer_topBarWin.first[1];
        char ch;

        if (!expression.empty()) {
            pageWin->changeTextAndPrint(expression);
        }
        if (!buffer.empty()) {
            pageWin->changeTextAndPrint(expression + "\n\n" + buffer);
        }

        while (true) {
            switch (int c = getch()) {
                case KEY_RESIZE: {
                    resize(expression + "\n\n" + buffer, buffer_topBarWin.first[0], buffer_topBarWin.second);
                    break;
                }
                case 'q':
                    goto freedom;
                case '0': case '1': case '2':
                case '3': case '4': case '5':
                case '6': case '7': case '8':
                case '9':
                    ch = c;
                    expression.push_back(ch);
                    pageWin->mvprintWithAttr(1, expression.size(), ch, WA_NORMAL);
                    break;
                case '*':
                    if (!is_star && !expression.empty()) {
                        expression.push_back('*');
                        pageWin->mvprintWithAttr(1, expression.size(), '*', WA_NORMAL);
                        is_star = true;
                    }
                    break;
                case KEY_BACKSPACE:
                    if (!expression.empty()) {
                        if (expression[expression.size()-1] == '*') {
                            is_star = false;
                        }
                        pageWin->mvprintWithAttr(1, expression.size(), ' ', WA_NORMAL);
                        expression.pop_back();
                    }
                    break;
                case KEY_ENTER_: {
                    if (is_star && expression[expression.size()-1] != '*') {
                        short pos = 0;
                        string operands[2];
                        for (const auto &chr: expression) {
                            if (chr == '*') {
                                pos = 1;
                                continue;
                            }
                            operands[pos].push_back(chr);
                        }

                        int a = stoi(operands[0]);
                        int b = stoi(operands[1]);

                        buffer.clear();
                        //pos = 1;
                        if (a & 1) {
                            vector<int> summands;
                            while (a > 1) {
                                if (a & 1) {
                                    summands.push_back(b);
                                }
                                buffer += to_string(a) + " * " + to_string(b) + "\n";
                                a /= 2; b *= 2;
                            }
                            buffer += to_string(a) + " * " + to_string(b) + "\n";
                            summands.push_back(b);

                            int ans = 0;
                            for (const auto& num : summands) {
                                buffer += to_string(num) + " + ";
                                ans += num;
                            }
                            buffer[buffer.size()-2] = '=';
                            buffer += to_string(ans);
                        } else {
                            while (a > 1) {
                                buffer += to_string(a) + " * " + to_string(b) + "\n";
                                a /= 2; b *= 2;
                            }
                            buffer += to_string(a) + " * " + to_string(b) + " = " + to_string(b);
                        }
                        pageWin->changeTextAndPrint(expression + "\n\n" + buffer);
                    }
                    break;
                }
                default:
                    break;
            }
        }
        freedom:
    }
};

extern "C" PluginInterface* getPluginClass(PageWindow* pageWin, TurnPageBarWindow* turnPageTabWin, WINDOW* rawTopBarWin, int& key) {
    key = PLUGIN_KEY;
    return new russDivideMethod(pageWin, turnPageTabWin, rawTopBarWin);
}
