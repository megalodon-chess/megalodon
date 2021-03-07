//
//  Megalodon
//  UCI chess engine
//  Copyright Patrick Huang and Arjun Sahlot 2021
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.
//

#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


bool bit(long long num, int pos) {
    return ((1LL << pos) & num) != 0;
}

string repr(long long board) {
    vector<string> rows;
    string final = "";

    for (auto y = 0; y < 8; y++) {
        string row = "";
        for (auto x = 0; x < 8; x++) {
            row += bit(board, 8*y+x) ? "1" : ".";
            row += " ";
        }
        rows.push_back(row);
    }

    for (auto i = 7; i >= 0; i--) {
        final += rows[i];
        final += "\n";
    }

    return final;
}
