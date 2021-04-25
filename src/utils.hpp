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

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "bitboard.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

double get_time();

string strip(const string&);
string replace(string, const string&);
string join(const string&, const vector<string>&);
vector<string> split(const string&, const string& = "");
bool startswith(const string&, const string&);
bool endswith(const string&, const string&);

vector<Position> flatten(const vector<vector<Position>>& vec);

string rand_choice(vector<string>);
U64 randull();

string str_variant(const Bitboard::Variant&);
Bitboard::Variant variant_str(const string&);
