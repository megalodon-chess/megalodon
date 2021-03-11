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
#include <utility>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::vector;
using std::string;

typedef unsigned long long U64;

struct Move {
    Move();
    Move(char, char, bool=false, char=0);

    char from;
    char to;
    bool is_promo;
    char promo;
};

struct Position {
    Position();
    Position(U64, U64, U64, U64, U64, U64, U64, U64, U64, U64, U64, U64, bool, char, bool, char);

    U64 wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk;
    bool turn;
    char castling;
    bool ep;
    char ep_square;
    float eval;
    vector<Move> move_stack;
};

Position copy_position(Position);
Move copy_move(Move);

namespace Bitboard {
    // Move format: vector<char> = {start square, end square, promotion piece}
    // Promotion piece format: 0=knight, 1=bishop, 2=rook, 3=queen

    constexpr U64 EMPTY = 0;

    constexpr U64 START_WP = 65280;
    constexpr U64 START_WN = 66;
    constexpr U64 START_WB = 36;
    constexpr U64 START_WR = 129;
    constexpr U64 START_WQ = 8;
    constexpr U64 START_WK = 16;

    constexpr U64 START_BP = 71776119061217280ULL;
    constexpr U64 START_BN = 4755801206503243776ULL;
    constexpr U64 START_BB = 2594073385365405696ULL;
    constexpr U64 START_BR = 9295429630892703744ULL;
    constexpr U64 START_BQ = 576460752303423488ULL;
    constexpr U64 START_BK = 1152921504606846976ULL;

    const vector<vector<char>> DIR_R = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_N = {{-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {1, -2}, {-1, 2}, {-1, -2}, {1, 2}};
    const vector<vector<char>> DIR_B = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const vector<vector<char>> DIR_Q = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_K = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    bool bit(U64, int);
    bool bit(char, int);
    char bit_pos(U64, bool=true);
    U64 set_bit(U64, int, bool);
    string board_str(U64, string="X", string="-");
    string move_str(Move);
    pair<char, char> first_bit(U64);

    U64 attacked(U64, U64, U64, U64, U64, U64, U64, bool);
    bool pinned(U64, U64, U64, U64, U64, U64, U64, U64);
    pair<U64, char> checkers(U64, U64, U64, U64, U64, U64, U64, bool);
    vector<Move> king_moves(U64, U64);
    vector<Move> legal_moves(Position);
    Position push(Position, Move);
}
