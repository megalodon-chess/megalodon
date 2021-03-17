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
    // todo pawn shield
    if (stage == 2) {
        return 0;
    } else {
        float rank_eval;
        if (kpos.y <= 3) rank_eval = 0.7 * (3-kpos.y);
        else rank_eval = 0.7 * (kpos.y-4);

        float col_eval;
        if (kpos.x <= 3) col_eval = 0.35 * (3-kpos.x);
        else col_eval = 0.35 * (kpos.x-4);

        return rank_eval+col_eval;
    }
}

float pawns(Options& options, U64 pawns, bool side) {
    vector<char> file_count = {0, 0, 0, 0, 0, 0, 0, 0};
    float score = 0;
    char num = 0;

    for (char i = 0; i < 64; i++) {
        if (Bitboard::bit(pawns, i)) {
            const char x = i%8, y = (side ? i/8 : 7-(i/8));
            file_count[x]++;
            num++;
            score += 0.45 * y;
        }
    }
    if (num != 0) score /= num;
    for (auto cnt: file_count) if (cnt >= 2) score -= 0.08*(cnt-1);

    return score;
}

float knights(Options& options, U64 knights) {
    char count = 0;
    float score = 0;
    for (char i = 0; i < 64; i++) {
        if (Bitboard::bit(knights, i)) {
            const char x = i%8, y = i/8;
            float horiz, vert;
            if (x <= 3) horiz = x / 2.1;
            else horiz = (7-x) / 2.1;
            if (y <= 3) vert = y / 2.1;
            else vert = (7-y) / 2.1;
            score += horiz * vert;
            count++;
        }
    }
    if (count != 0) score /= count;

    return score;
}


float center_control(Options& options, Position pos) {
    float inner_pop = 0;
    float inner_pop_wp = 0;
    float inner_pop_bp = 0;
    inner_pop_wp += Bitboard::popcnt(pos.wp & INNER_CENTER) * 1.5;
    inner_pop += Bitboard::popcnt(pos.wn & INNER_CENTER) * 1.2;
    inner_pop += Bitboard::popcnt(pos.wb & INNER_CENTER);
    inner_pop += Bitboard::popcnt(pos.wr & INNER_CENTER) * 0.8;
    inner_pop += Bitboard::popcnt(pos.wq & INNER_CENTER);
    inner_pop_bp -= Bitboard::popcnt(pos.bp & INNER_CENTER) * 1.5;
    inner_pop -= Bitboard::popcnt(pos.bn & INNER_CENTER) * 1.2;
    inner_pop -= Bitboard::popcnt(pos.bb & INNER_CENTER);
    inner_pop -= Bitboard::popcnt(pos.br & INNER_CENTER) * 0.8;
    inner_pop -= Bitboard::popcnt(pos.bq & INNER_CENTER);
    inner_pop /= INNER_COUNT;
    if (inner_pop_wp == 0) inner_pop -= 1;
    if (inner_pop_bp == 0) inner_pop += 1;
    inner_pop += inner_pop_wp;
    inner_pop += inner_pop_bp;

    float outer_pop = 0;
    outer_pop += Bitboard::popcnt(pos.wp & OUTER_CENTER) * 1.5;
    outer_pop += Bitboard::popcnt(pos.wn & OUTER_CENTER) * 1.2;
    outer_pop += Bitboard::popcnt(pos.wb & OUTER_CENTER);
    outer_pop += Bitboard::popcnt(pos.wr & OUTER_CENTER) * 0.8;
    outer_pop += Bitboard::popcnt(pos.wq & OUTER_CENTER);
    outer_pop -= Bitboard::popcnt(pos.bp & OUTER_CENTER) * 1.5;
    outer_pop -= Bitboard::popcnt(pos.bn & OUTER_CENTER) * 1.2;
    outer_pop -= Bitboard::popcnt(pos.bb & OUTER_CENTER);
    outer_pop -= Bitboard::popcnt(pos.br & OUTER_CENTER) * 0.8;
    outer_pop -= Bitboard::popcnt(pos.bq & OUTER_CENTER);
    outer_pop /= OUTER_COUNT;

    U64 w_attacks = Bitboard::attacked(pos, true);
    U64 b_attacks = Bitboard::attacked(pos, false);
    float inner_attack = Bitboard::popcnt(w_attacks&INNER_CENTER) - Bitboard::popcnt(b_attacks&INNER_CENTER);
    float outer_attack = Bitboard::popcnt(w_attacks&OUTER_CENTER) - Bitboard::popcnt(b_attacks&OUTER_CENTER);
    inner_attack /= INNER_COUNT;
    outer_attack /= OUTER_COUNT;

    return inner_pop/1.5 + outer_pop/2 + inner_attack + outer_attack/1.6;
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
    const float wpawn = pawns(options, pos.wp, true);
    const float bpawn = pawns(options, pos.bp, false);
    const float wknight = knights(options, pos.wn);
    const float bknight = knights(options, pos.bn);

    const float cent = center_control(options, pos);

    return mat + 1.2*cent + 0.9*(wking-bking) + 0.8*(wpawn-bpawn) + 0.9*(wknight-bknight);
}
