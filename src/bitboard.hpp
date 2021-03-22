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
#include <tuple>
#include <string>
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::pair;
using std::tuple;
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
    float alpha;
    float beta;
    bool done;  // Used by search
    Position* parent;
    vector<Move> move_stack;
};

struct Location {
    Location();
    Location(char, char);

    char x;
    char y;
};

const char popcnt_tbl[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8,
};

namespace Bitboard {
    // Promotion piece format: 0=knight, 1=bishop, 2=rook, 3=queen

    constexpr U64 EMPTY = 0ULL;
    constexpr U64 FULL = 18446744073709551615ULL;

    constexpr U64 START_WP = 65280ULL;
    constexpr U64 START_WN = 66ULL;
    constexpr U64 START_WB = 36ULL;
    constexpr U64 START_WR = 129ULL;
    constexpr U64 START_WQ = 8ULL;
    constexpr U64 START_WK = 16ULL;

    constexpr U64 START_BP = 71776119061217280ULL;
    constexpr U64 START_BN = 4755801206503243776ULL;
    constexpr U64 START_BB = 2594073385365405696ULL;
    constexpr U64 START_BR = 9295429630892703744ULL;
    constexpr U64 START_BQ = 576460752303423488ULL;
    constexpr U64 START_BK = 1152921504606846976ULL;

    constexpr U64 CASTLING_BK = 8070450532247928832ULL;
    constexpr U64 CASTLING_BQ = 2017612633061982208ULL;
    constexpr U64 CASTLING_WQ = 28ULL;
    constexpr U64 CASTLING_WK = 112ULL;

    const vector<vector<char>> DIR_R = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_N = {{-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {1, -2}, {-1, 2}, {-1, -2}, {1, 2}};
    const vector<vector<char>> DIR_B = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const vector<vector<char>> DIR_Q = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_K = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    bool bit(const U64&, const int&);
    bool bit(const char&, const int&);
    char popcnt(const U64&);
    void set_bit(U64&, int);
    void unset_bit(U64&, int);
    void set_bit(char&, int);
    void unset_bit(char&, int);
    bool contains(const vector<vector<char>>&, const vector<char>&);
    bool in_board(const char& x, const char& y);
    Location first_bit(const U64&);

    string piece_at(const Position&, const char&);
    U64 color(const Position&, const bool&);
    string board_str(const U64&, const string="X", const string="-");
    string board_str(const Position&);
    string square_str(const char&);
    string move_str(const Move&);
    string fen(const Position&);
    Position parse_fen(const string&);
    Move parse_uci(const string&);

    U64 attacked(const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const bool&);
    U64 attacked(const Position&, const bool&);
    tuple<bool, U64> pinned(const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const U64&);
    tuple<U64, char> checkers(const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const U64&, const bool&);
    vector<Move> king_moves(const Position&, const U64&, const U64&, const U64&);
    vector<Move> legal_moves(Position, const U64&);
    vector<Move> order_moves(const Position&, const vector<Move>&, const U64&);
    int quick_eval(const Position&, const Move&, const U64&);

    vector<U64*> bb_pointers(Position&);
    Position startpos();
    Position push(Position, const Move&);
    Position push(Position, string);
}
