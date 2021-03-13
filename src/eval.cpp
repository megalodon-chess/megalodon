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
#include <queue>
#include <string>
#include "bitboard.hpp"
#include "options.hpp"
#include "eval.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


float material(Position pos) {
    float value = 0;
    for (auto i = 0; i < 64; i++) {
        if (Bitboard::bit(pos.wp, i)) value += 1;
        else if (Bitboard::bit(pos.wn, i)) value += 3;
        else if (Bitboard::bit(pos.wb, i)) value += 3;
        else if (Bitboard::bit(pos.wr, i)) value += 5;
        else if (Bitboard::bit(pos.wq, i)) value += 9;
        else if (Bitboard::bit(pos.bp, i)) value -= 1;
        else if (Bitboard::bit(pos.bn, i)) value -= 3;
        else if (Bitboard::bit(pos.bb, i)) value -= 3;
        else if (Bitboard::bit(pos.br, i)) value -= 5;
        else if (Bitboard::bit(pos.bq, i)) value -= 9;
    }
    return value;
}

float material_weight(Options& options, int movect) {
    return (float)options.EvalMaterial / 100;
}


float center(U64 w_attacks, U64 b_attacks) {
    float inner = 0, outer = 0;

    inner += (float)Bitboard::bit_count(w_attacks&INNER_CENTER) / Bitboard::bit_count(INNER_CENTER);
    outer += (float)Bitboard::bit_count(w_attacks&OUTER_CENTER) / Bitboard::bit_count(OUTER_CENTER);

    inner -= (float)Bitboard::bit_count(b_attacks&INNER_CENTER) / Bitboard::bit_count(INNER_CENTER);
    outer -= (float)Bitboard::bit_count(b_attacks&OUTER_CENTER) / Bitboard::bit_count(OUTER_CENTER);

    return inner*INNER_WEIGHT + outer*OUTER_WEIGHT;
}

float center_weight(Options& options, int movect) {
    return (float)options.EvalCenter / 100;
}


float eval(Options& options, Position pos, bool attacks, U64 w_attacks, U64 b_attacks) {
    if (!attacks) {
        w_attacks = Bitboard::attacked(pos, true);
        b_attacks = Bitboard::attacked(pos, false);
    }
    int movect = pos.move_stack.size();

    float mat;
    float mat_weight = material_weight(options, movect);
    if (mat_weight == 0) mat = 0;
    else mat = material(pos);

    float cent;
    float cent_weight = center_weight(options, movect);
    if (cent_weight == 0) cent = 0;
    else cent = center(w_attacks, b_attacks);

    return mat*mat_weight + cent*cent_weight;
}
