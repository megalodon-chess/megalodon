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
#include "search.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


float material(Position pos) {
    float value = 0;
    for (auto i = 0; i < 64; i++) {
        if      (Bitboard::bit(pos.wp, i)) value += 1;
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

float total_mat(Position pos) {
    float value = 0;
    for (auto i = 0; i < 64; i++) {
        if      (Bitboard::bit(pos.wp, i)) value += 1;
        else if (Bitboard::bit(pos.wn, i)) value += 3;
        else if (Bitboard::bit(pos.wb, i)) value += 3;
        else if (Bitboard::bit(pos.wr, i)) value += 5;
        else if (Bitboard::bit(pos.wq, i)) value += 9;
        else if (Bitboard::bit(pos.bp, i)) value += 1;
        else if (Bitboard::bit(pos.bn, i)) value += 3;
        else if (Bitboard::bit(pos.bb, i)) value += 3;
        else if (Bitboard::bit(pos.br, i)) value += 5;
        else if (Bitboard::bit(pos.bq, i)) value += 9;
    }
    return value;
}


float king(Options& options, char stage, Location kpos, U64 pawns, U64 others) {
    if (stage == 2) {
        return 0;
    } else {
        float rank_eval;
        if (kpos.y <= 3) rank_eval = 0.6 * (3-kpos.y);
        else rank_eval = 0.6 * (kpos.y-4);

        float col_eval;
        if (kpos.x <= 3) col_eval = 0.35 * (3-kpos.x);
        else col_eval = 0.35 * (kpos.x-4);

        return rank_eval+col_eval;
    }
}


float eval(Options& options, Position pos, bool moves_exist) {
    if (!moves_exist) {
        if (pos.turn) return MIN;
        else return MAX;
    }

    const int movect = pos.move_stack.size();
    const float total = total_mat(pos);
    char stage;  // 0 = opening, 1 = middlegame, 2 = endgame
    if (total > 68) stage = 0;
    else if (20 < total && total <= 68) stage = 1;
    else stage = 2;

    const float mat = material(pos);
    const float wking = king(options, stage, Bitboard::first_bit(pos.wk), pos.wp, Bitboard::color(pos, false));
    const float bking = king(options, stage, Bitboard::first_bit(pos.bk), pos.bp, Bitboard::color(pos, true));

    return mat + (wking-bking);
}
