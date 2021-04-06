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

struct MoveOrder {
    MoveOrder();

    bool computed;
    char movecnt;
    Move moves[Bitboard::MAX_HASH_MOVES];
};

class Options {
/*
Hash: type=spin, default=16, min=1, max=65536, hash table size (megabytes)
UseHashTable: type=check, default=true, whether the engine should use hash table.
HashStart: type=spin, default=5, min=1, max=8, starting depth to read and write into hash table.

MoveTimeMult: type=spin, default=100, min=10, max=1000, multiplier (percent) of move time.
UseEndgame: type=check, default=true, whether to use endgame algorithms.
LMRFactor: type=spin, default=0, min=0, max=100, percent of lowest branches to prune. ONLY WORKS IF USING HASH TABLE
QuickMove: type=check, default=true, whether to move immediately when there is only one legal move (output will be missing eval info).

EvalMaterial: type=spin, default=100, min=0, max=1000, weight (percent) of material eval.
EvalPawnStruct: type=spin, default=100, min=0, max=1000, weight (percent) of pawn structure eval.
EvalKnights: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.
EvalKings: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.

PrintCurrMove: type=check, default=true, whether the engine should print currmove info.
PrintPv: type=check, default=true, whether the engine should print principal variation.
Chat: type=check, default=true, whether the engine should chat with you.
*/

public:
    Options();
    void set_hash();
    void clear_hash();

    MoveOrder* hash_table;
    int hash_size;

    int Hash;
    bool UseHashTable;
    int HashStart;

    int MoveTimeMult;
    bool UseEndgame;
    int LMRFactor;
    bool QuickMove;

    float EvalMaterial;
    float EvalPawnStruct;
    float EvalSpace;
    float EvalKnights;
    float EvalKings;

    bool PrintCurrMove;
    bool PrintPv;
    bool Chat;
};
