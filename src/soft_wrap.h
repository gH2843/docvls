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
#ifndef P_AML_SOFT_WRAP_H
#define P_AML_SOFT_WRAP_H

#include <string>

using namespace std;

string softWrap(const string& page, short col) {
    int pos = 0, mod = 0;
    string buff;
    --col; // todo: how to optimize it?
    for (; pos < page.size();) {
        int pose = pos + col;
        for (;; ++pos) {
            buff += page[pos];
            if (page[pos] == '\n') {
                buff += "*";
                mod = 0;
                ++pos;
                break;
            } else if (pos == pose - mod) {
                if (isalpha(page[pos+1]) && isalpha(page[pos])) {
                    buff += "\n*-";
                    mod = 1;
                } else {
                    buff += "\n*";
                    mod = 0;
                }
                ++pos;
                break;
            }
        }
    }
    return buff;
}

#endif //P_AML_SOFT_WRAP_H