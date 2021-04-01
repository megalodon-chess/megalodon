//
//  Megalodon
//  UCI chess engine
//  Copyright the Megalodon developers
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
#include <algorithm>
#include <string>
#include <chrono>
#include "bitboard.hpp"
#include "utils.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


double get_time() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    double elapse = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    return elapse / 1000;
}


string strip(string str) {
    int start = str.find_first_not_of(" ");
    int end = str.find_last_not_of(" ");

    if (start == string::npos) start = 0;
    if (end == string::npos) end = str.size()-1;
    return str.substr(start, end-start+1);
}

string replace(string str, string rep) {
    int size = rep.size();
    while (true) {
        if (str.size() == 0) break;

        int pos = str.find(rep);
        if (pos == string::npos) break;

        if (pos > 0) str = str.substr(0, pos) + str.substr(pos+size);
        else if (pos+size < str.size()) str = str.substr(pos+size);
        else str = "";
    }
    return str;
}

string join(string delim, vector<string> parts) {
    string str;
    for (auto i = 0; i < parts.size()-1; i++) {
        str += parts[i];
        str += delim;
    }
    str += parts[parts.size()-1];
    return str;
}

vector<string> split(string str, string delim) {
    vector<string> parts;
    int prev = 0, pos = 0;
    do {
        pos = str.find(delim, prev);
        if (pos == string::npos) pos = str.length();
        string part = str.substr(prev, pos-prev);
        if (!part.empty()) parts.push_back(part);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return parts;
}

bool startswith(string str, string prefix) {
    if (str.size() < prefix.size()) return false;
    return (str.substr(0, prefix.size()) == prefix);
}

bool endswith(string str, string suffix) {
    if (str.size() < suffix.size()) return false;
    return (str.substr(str.size()-suffix.size(), suffix.size()) == suffix);
}


vector<Position> flatten(vector<vector<Position>> vec) {
    vector<Position> flat;
    for (auto v: vec) flat.insert(flat.end(), v.begin(), v.end());
    return flat;
}


string rand_choice(vector<string> choices) {
    return choices[rand()%choices.size()];
}

U64 randull() {
    return ((U64)rand()) + (((U64)rand())<<32);
}
