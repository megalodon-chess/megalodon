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

using Bitboard::popcnt;
using Bitboard::bit;


float material(const Position& pos) {
    float value = 0;
    value += popcnt(pos.wp) * 1;
    value += popcnt(pos.wn) * 3;
    value += popcnt(pos.wb) * 3;
    value += popcnt(pos.wr) * 5;
    value += popcnt(pos.wq) * 9;
    value -= popcnt(pos.bp) * 1;
    value -= popcnt(pos.bn) * 3;
    value -= popcnt(pos.bb) * 3;
    value -= popcnt(pos.br) * 5;
    value -= popcnt(pos.bq) * 9;
    return value;
}

float total_mat(const Position& pos) {
    float value = 0;
    value += popcnt(pos.wp) * 1;
    value += popcnt(pos.wn) * 3;
    value += popcnt(pos.wb) * 3;
    value += popcnt(pos.wr) * 5;
    value += popcnt(pos.wq) * 9;
    value += popcnt(pos.bp) * 1;
    value += popcnt(pos.bn) * 3;
    value += popcnt(pos.bb) * 3;
    value += popcnt(pos.br) * 5;
    value += popcnt(pos.bq) * 9;
    return value;
}

float non_pawn_mat(const Position& pos) {
    float value = 0;
    value += popcnt(pos.wn) * 3;
    value += popcnt(pos.wb) * 3;
    value += popcnt(pos.wr) * 5;
    value += popcnt(pos.wq) * 9;
    value += popcnt(pos.bn) * 3;
    value += popcnt(pos.bb) * 3;
    value += popcnt(pos.br) * 5;
    value += popcnt(pos.bq) * 9;
    return value;
}


float king(const Options& options, const char& stage, const Location& kpos, const U64& pawns, const U64& others) {
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

float pawns(const Options& options, const U64& pawns, const bool& side) {
    vector<char> file_count = {0, 0, 0, 0, 0, 0, 0, 0};
    float score = 0;
    char num = 0;

    for (char i = 0; i < 64; i++) {
        if (bit(pawns, i)) {
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

float knights(const Options& options, const U64& kn) {
    char count = 0;
    float score = 0;
    for (char i = 0; i < 64; i++) {
        if (bit(kn, i)) {
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

float rooks(const Options& options, const U64& ro) {
    char count = 0;
    float score = 0;
    for (char i = 0; i < 64; i++) {
        if (bit(ro, i)) {
            const char x = i%8;
            if (x <= 3) score += x / 2.1;
            else score += (7-x) / 2.1;
            count++;
        }
    }
    if (count != 0) score /= count;

    return score;
}

float queens(const Options& options, const U64& qu) {
    char count = 0;
    float score = 0;
    for (char i = 0; i < 64; i++) {
        if (bit(qu, i)) {
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


float center_control(const Options& options, const Position& pos, const int& stage) {
    float w_inneratt = popcnt(Bitboard::attacked(pos, true)&IN_CENT);
    float b_inneratt = popcnt(Bitboard::attacked(pos, false)&IN_CENT);
    float w_innerpop = popcnt(pos.wn&IN_CENT) + popcnt(pos.wb&IN_CENT) + popcnt(pos.wr&IN_CENT) + popcnt(pos.wq&IN_CENT);
    float b_innerpop = popcnt(pos.bn&IN_CENT) + popcnt(pos.bb&IN_CENT) + popcnt(pos.br&IN_CENT) + popcnt(pos.bq&IN_CENT);
    float wp_innerpop = popcnt(pos.wp&IN_CENT);
    float bp_innerpop = popcnt(pos.bp&IN_CENT);

    // float w_outeratt = popcnt(Bitboard::attacked(pos, true)&OUT_CENT);
    // float b_outeratt = popcnt(Bitboard::attacked(pos, false)&OUT_CENT);
    // float w_outerpop = popcnt(pos.wn&OUT_CENT) + popcnt(pos.wb&OUT_CENT) + popcnt(pos.wr&OUT_CENT) + popcnt(pos.wq&OUT_CENT);
    // float b_outerpop = popcnt(pos.bn&OUT_CENT) + popcnt(pos.bb&OUT_CENT) + popcnt(pos.br&OUT_CENT) + popcnt(pos.bq&OUT_CENT);
    // float wp_outerpop = popcnt(pos.wp&OUT_CENT);
    // float bp_outerpop = popcnt(pos.bp&OUT_CENT);

    float score = 0;
    score += w_inneratt / IN_CNT * 1.3;
    score -= b_inneratt / IN_CNT * 1.3;
    score += wp_innerpop / IN_CNT * 1.5;
    if (wp_innerpop != 0 || stage != 0) score += w_innerpop / IN_CNT * 1.3;
    score -= bp_innerpop / IN_CNT * 1.5;
    if (bp_innerpop != 0 || stage != 0) score -= b_innerpop / IN_CNT * 1.3;

    // score += w_outeratt / OUT_CNT * 0.6;
    // score -= b_outeratt / OUT_CNT * 0.6;
    // score += wp_outerpop / OUT_CNT * 0.8;
    // score += w_outerpop / OUT_CNT * 0.6;
    // score -= bp_outerpop / OUT_CNT * 0.8;
    // score -= b_outerpop / OUT_CNT * 0.6;

    return score;
}


float eval(const Options& options, const Position& pos, const bool& moves_exist, const int& depth, const U64& o_attacks) {
    if (!moves_exist) {
        bool checked;
        if (pos.turn && ((o_attacks & pos.wk) != 0)) checked = true;
        else if (!pos.turn && ((o_attacks & pos.bk) != 0)) checked = true;
        if (checked) {
            // Increment value by depth to encourage sooner mate.
            if (pos.turn) return MIN+depth;
            else return MAX-depth;
        }
        return 0;
    }
    return material(pos);

    // const int movect = pos.move_stack.size();
    // const float total = total_mat(pos);
    // char stage;  // 0 = opening, 1 = middlegame, 2 = endgame
    // if (total > 68) stage = 0;
    // else if (20 < total && total <= 68) stage = 1;
    // else stage = 2;

    // const float mat = material(pos);
    // const float cent = center_control(options, pos, stage);

    // const float wking = king(options, stage, Bitboard::first_bit(pos.wk), pos.wp, Bitboard::color(pos, false));
    // const float bking = king(options, stage, Bitboard::first_bit(pos.bk), pos.bp, Bitboard::color(pos, true));
    // const float wpawn = pawns(options, pos.wp, true);
    // const float bpawn = pawns(options, pos.bp, false);
    // const float wknight = knights(options, pos.wn);
    // const float bknight = knights(options, pos.bn);
    // const float wrook = rooks(options, pos.wr);
    // const float brook = rooks(options, pos.br);
    // const float wqueen = queens(options, pos.wq);
    // const float bqueen = queens(options, pos.bq);

    // return (
    //     options.EvalMaterial/100 * 1.0 *  mat//                 +
        // options.EvalCenter  /100 * 0.3 *  cent               +
        // options.EvalKing    /100 * 0.7 * (wking   - bking)   +
        // options.EvalPawn    /100 * 1.0 * (wpawn   - bpawn)   +
        // options.EvalKnight  /100 * 0.6 * (wknight - bknight) +
        // options.EvalRook    /100 * 0.3 * (wrook   - brook)   +
        // options.EvalQueen   /100 * 0.2 * (wqueen  - bqueen)
    // );
}
