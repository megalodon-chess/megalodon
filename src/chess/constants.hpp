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

#pragma once

#include <iostream>
#include <vector>
#include <string>

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


constexpr int EM = 0;

constexpr int WP = 1;
constexpr int WN = 2;
constexpr int WB = 3;
constexpr int WR = 4;
constexpr int WQ = 5;
constexpr int WK = 6;

constexpr int BP = 11;
constexpr int BN = 12;
constexpr int BB = 13;
constexpr int BR = 14;
constexpr int BQ = 15;
constexpr int BK = 16;

const string CASTLING_SYMBOLS = "KQkq";
const string BOARD_OUTROW = "+---+---+---+---+---+---+---+---+";
const string BOARD_OUTCOL = " | ";

const vector<vector<int>> DIR_R = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
const vector<vector<int>> DIR_N = {};
const vector<vector<int>> DIR_B = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
const vector<vector<int>> DIR_Q = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}};

