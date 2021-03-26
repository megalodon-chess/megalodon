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


Options::Options() {
    Hash = 16;
    UseHashTable = false;
    PrintCurrMove = true;
    MoveTimeMult = 100;
    EvalMaterial = 100;
    Chat = false;

    set_hash();
}

void Options::set_hash() {
    delete hash_evaled;
    delete hash_evals;
    hash_size = Hash * 125000;

    hash_evaled = new bool[hash_size];
    hash_evals = new float[hash_size];
    for (int i = 0; i < hash_size; i++) {
        hash_evaled[i] = false;
        hash_evals[i] = 0;
    }
}
