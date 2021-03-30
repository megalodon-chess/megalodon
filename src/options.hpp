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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

class Options {
/*
Hash: type=spin, default=16, min=1, max=65536, hash table size (megabytes)
UseHashTable: type=check, default=false, whether the engine should use hash table.

ABPassStart: type=spin, default=5, min=1, max=100, start depth where alpha and beta values are passed to next iteration.
ABPassMargin: type=spin, default=500, min=0, max=10000, add/sub offset of alpha and beta.
MoveTimeMult: type=spin, default=100, min=10, max=1000, multiplier (percent) of move time.

EvalMaterial: type=spin, default=100, min=0, max=1000, weight (percent) of material eval.
EvalPawnStruct: type=spin, default=100, min=0, max=1000, weight (percent) of pawn structure eval.
EvalKnights: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.
EvalKings: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.

PrintCurrMove: type=check, default=true, whether the engine should print currmove info.
PrintPv: type=check, default=true, whether the engine should print prinipal variation.
Chat: type=check, default=true, whether the engine should chat with you.
*/

public:
    Options();
    void set_hash();

    bool* hash_evaled = new bool[16];
    float* hash_evals = new float[16];
    int hash_size;

    int Hash;
    bool UseHashTable;

    int ABPassStart;
    int ABPassMargin;
    int MoveTimeMult;

    int EvalMaterial;
    int EvalPawnStruct;
    int EvalSpace;
    int EvalKnights;
    int EvalKings;

    bool PrintCurrMove;
    bool PrintPv;
    bool Chat;
};
