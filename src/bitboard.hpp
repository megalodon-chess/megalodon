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
using std::endl;
using std::vector;
using std::string;

namespace Bitboard {
    constexpr unsigned long long START_WP = 65280;
    constexpr unsigned long long START_WN = 66;
    constexpr unsigned long long START_WB = 36;
    constexpr unsigned long long START_WR = 129;
    constexpr unsigned long long START_WQ = 8;
    constexpr unsigned long long START_WK = 16;

    constexpr unsigned long long START_BP = 71776119061217280;
    constexpr unsigned long long START_BN = 4755801206503243776;
    constexpr unsigned long long START_BB = 2594073385365405696;
    constexpr unsigned long long START_BR = 9295429630892703744;
    constexpr unsigned long long START_BQ = 576460752303423488;
    constexpr unsigned long long START_BK = 1152921504606846976;

    const vector<vector<int>> DIR_R = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<int>> DIR_N = {{-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {1, -2}, {-1, 2}, {-1, -2}, {1, 2}};
    const vector<vector<int>> DIR_B = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const vector<vector<int>> DIR_Q = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<int>> DIR_K = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    bool bit(long long, int);
    long long set_bit(long long, int, bool);
    long long push(long long, vector<char>);
    string board_str(long long);

    vector<vector<char>> knight_moves(long long, long long, long long);
    vector<vector<char>> king_moves(long long, long long, long long);
    vector<vector<char>> rook_moves(long long, long long, long long);
    vector<vector<char>> bishop_moves(long long, long long, long long);
    vector<vector<char>> queen_moves(long long, long long, long long);

    const string BOARD_OUTROW = "+---+---+---+---+---+---+---+---+";
    const string BOARD_OUTCOL = " | ";
}
