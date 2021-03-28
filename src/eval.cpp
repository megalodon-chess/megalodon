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


float phase(const Position& pos) {
    // 1 = full middlegame, 0 = full endgame.
    float npm = non_pawn_mat(pos);
    if (npm >= MIDGAME_LIM) return 1;
    else if (npm <= ENDGAME_LIM) return 0;
    else return ((float)(npm-ENDGAME_LIM) / (MIDGAME_LIM-ENDGAME_LIM));
}


float middle_game(const float& pawn_struct) {
    return (
        pawn_struct * 0.9
    );
}

float end_game(const float& pawn_struct) {
    return (
        pawn_struct * 1.2
    );
}


float pawn_structure(const U64& s_pawns, const U64& o_pawns) {
    char passed = 0;
    char backward = 0;
    char islands = 0;
    char doubled = 0;

    // Islands and doubled/tripled
    bool on = false;
    for (char i = 0; i < 8; i++) {
        const U64 file = Bitboard::FILES[i] & s_pawns;
        if (file == Bitboard::EMPTY) {
            on = false;
        } else {
            if (!on) islands++;
            on = true;
        }

        const char wcnt = popcnt(file);
        if (wcnt >= 2) doubled += (wcnt-1);
    }

    return (
        -0.3 * islands +
        -0.2 * doubled +
         0.4 * passed  +
        -0.2 * backward
    );
}

float space(const U64& s_pawns, const U64& o_pawns, const char& pawn_dir, const vector<Move>& moves, const bool& side) {
    char blocked = 0;
    const int pawn_cnt = popcnt(s_pawns) + popcnt(o_pawns);

    for (char i = 0; i < 64; i++) {
        if (bit(s_pawns, i) && (bit(o_pawns, i-8*pawn_dir) || (bit(o_pawns, i-17*pawn_dir) && bit(o_pawns, i-15*pawn_dir)))) blocked++;
        if (bit(o_pawns, i) && (bit(s_pawns, i+8*pawn_dir) || (bit(s_pawns, i+15*pawn_dir) && bit(s_pawns, i+17*pawn_dir)))) blocked++;
    }

    const int weight = pawn_cnt - 3 + blocked;

    float space = 0;
    for (char x = 2; x < 6; x++) {
        for (char y = side ? 1 : 2; y < side ? 6 : 7; y++) {
            const char loc = y<<3 + x;
            if (!bit(s_pawns, loc) && !bit(o_pawns, loc+17*pawn_dir) && !bit(o_pawns, loc+15*pawn_dir)) {
                space++;
                if (bit(s_pawns, loc-8*pawn_dir) || bit(s_pawns, loc-16*pawn_dir) || bit(s_pawns, loc-24*pawn_dir) &&
                    Bitboard::num_attacks(moves, Location(loc)) == 0) space++;
            }
        }
    }

    return space*weight*weight/16;
}


float eval(const Options& options, const Position& pos, const vector<Move>& moves, const int& depth, const U64& o_attacks) {
    if (moves.empty()) {
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

    const float mat = material(pos);
    const float pawn_struct = ((float)options.EvalPawnStruct)/100 * (pawn_structure(pos.wp, pos.bp)-pawn_structure(pos.bp, pos.wp));

    // Endgame and middle game are for weighting categories.
    const float mg = middle_game(pawn_struct);
    const float eg = end_game(pawn_struct);
    const float p = phase(pos);
    const float score = mg*p + eg*(1-p);

    return mat + score;
}
