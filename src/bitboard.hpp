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
    vector<Move> move_stack;
    bool done;   // Variable used by search algorithm.
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

    constexpr U64 CASTLING_BK = 6917529027641081856ULL;
    constexpr U64 CASTLING_BQ = 864691128455135232ULL;
    constexpr U64 CASTLING_WQ = 12ULL;
    constexpr U64 CASTLING_WK = 96ULL;

    const vector<vector<char>> DIR_R = {{0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_N = {{-2, 1}, {2, 1}, {-2, -1}, {2, -1}, {1, -2}, {-1, 2}, {-1, -2}, {1, 2}};
    const vector<vector<char>> DIR_B = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};
    const vector<vector<char>> DIR_Q = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}, {0, 1}, {-1, 0}, {1, 0}, {0, -1}};
    const vector<vector<char>> DIR_K = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    bool bit(U64, int);
    bool bit(char, int);
    char popcnt(U64);
    void set_bit(U64&, int);
    void unset_bit(U64&, int);
    void set_bit(char&, int);
    void unset_bit(char&, int);
    bool contains(vector<vector<char>>, vector<char>);
    Location first_bit(U64);

    string piece_at(Position, char);
    U64 color(Position, bool);
    string board_str(U64, string="X", string="-");
    string board_str(Position);
    string square_str(char);
    string move_str(Move);
    string fen(Position);
    Position parse_fen(string);
    Move parse_uci(string);

    U64 attacked(U64, U64, U64, U64, U64, U64, U64, bool);
    U64 attacked(Position, bool);
    tuple<bool, U64> pinned(U64, U64, U64, U64, U64, U64, U64, U64);
    tuple<U64, char> checkers(U64, U64, U64, U64, U64, U64, U64, U64, bool);
    vector<Move> king_moves(Position, U64, U64, U64);
    vector<Move> legal_moves(Position, U64);
    vector<Move> order_moves(const Position&, const vector<Move>&, const U64&);
    int quick_eval(const Position&, const Move&, const U64&);

    vector<U64*> bb_pointers(Position&);
    Position startpos();
    Position push(Position, Move);
    Position push(Position, string);
}
