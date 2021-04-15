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

struct Transposition {
    Transposition();

    char depth;
    char from;     // First six bits = square, last two = promo piece
    char to;       // First six bits = square, seventh = is_promo
    float eval;
    U64 hash;
};

class Options {
/*
Hash: type=spin, default=256, min=1, max=8192, hash table size (megabytes)
HashClearThres: type=spin, default=75, min=0, max=100, threshold (percent) of hashfull to clear hash.
TableLookupThres: type=spin, default=18, min=0, max=78, threshold of material remanining to lookup eval in hash table.

EvalMaterial: type=spin, default=100, min=0, max=1000, weight (percent) of material eval.
EvalSpace: type=spin, default=100, min=0, max=1000, weight (percent) of space eval.
EvalPawnStruct: type=spin, default=100, min=0, max=1000, weight (percent) of pawn structure eval.
EvalKnights: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.
EvalRooks: type=spin, default=100, min=0, max=1000, weight (percent) of rook eval.
EvalQueens: type=spin, default=100, min=0, max=1000, weight (percent) of queen eval.
EvalKings: type=spin, default=100, min=0, max=1000, weight (percent) of king eval.
*/

public:
    Options();
    void set_hash();

    Transposition* hash_table;
    U64 hash_size;
    U64 hash_filled;

    int Hash;
    int HashClearThres;
    int TableLookupThres;

    float EvalMaterial;
    float EvalSpace;
    float EvalPawnStruct;
    float EvalKnights;
    float EvalRooks;
    float EvalQueens;
    float EvalKings;
};
