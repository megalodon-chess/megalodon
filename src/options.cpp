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
#include <string>
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


MoveOrder::MoveOrder() {
    computed = false;
}


Options::Options() {
    Hash           = 256;
    UseHashTable   = true;
    HashStart      = 5;

    MoveTimeMult   = 100;
    UseEndgame     = true;
    LMRFactor      = 0;
    QuickMove      = true;

    EvalMaterial   = 1;
    EvalPawnStruct = 1;
    EvalSpace      = 1;
    EvalKnights    = 1;
    EvalKings      = 1;

    PrintCurrMove  = true;
    PrintPv        = true;
    Chat           = false;

    hash_table = new MoveOrder[16];
    set_hash();
}

void Options::set_hash() {
    delete[] hash_table;
    hash_size = Hash * 8530;
    hash_table = new MoveOrder[hash_size];
}

void Options::clear_hash() {
    for (auto i = 0; i < hash_size; i++) {
        hash_table[i].computed = false;
    }
}
