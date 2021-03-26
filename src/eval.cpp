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
#include <algorithm>
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


float pawn_structure(const U64& s_pawns, const U64& o_pawns, const bool& side) {
    char passed = 0;
    char backward = 0;
    char islands = 0;
    char doubled = 0;

    // Create files
    // todo use set
    vector<vector<char>> s_files(8), o_files(8);       // Store y values of all pawns in file

    for (char i = 0; i < 64; i++) {
        if      (bit(s_pawns, i)) s_files[i&7].push_back(i>>3);
        else if (bit(o_pawns, i)) o_files[i&7].push_back(i>>3);
    }

    // Islands and doubled/tripled
    bool on = false;
    for (char i = 0; i < 8; i++) {
        const vector<char> s_file = s_files[i], o_file = o_files[i];

        if (s_file.empty()) {
            on = false;
        } else {
            if (!on) islands++;
            on = true;
        }

        const char wcnt = s_file.size();
        if (wcnt >= 2) doubled += (wcnt-1);
    }

    // Check outer files
    if (!s_files[0].empty()) {
        if (o_files[0].empty() && o_files[1].empty()) passed++;
        else if (!o_files[1].empty()) {
            char to_check, s_target, o_target;
            if (side) {
                to_check = *std::min_element(s_files[0].begin(), s_files[0].end());       // Is this pawn backward?
                if (s_files[0].empty()) s_target = 7;                                     // Does it have a neighboring pawn behind it?
                else s_target = *std::min_element(s_files[1].begin(), s_files[1].end());
                o_target = *std::min_element(o_files[1].begin(), o_files[1].end());       // Does it have a opposite pawn blocking its path?
                if (s_target > to_check && o_target == s_target + 1) backward++;
            } else {
                to_check = *std::max_element(s_files[0].begin(), s_files[0].end());
                if (s_files[0].empty()) s_target = 0;
                else s_target = *std::max_element(s_files[1].begin(), s_files[1].end());
                o_target = *std::max_element(o_files[1].begin(), o_files[1].end());
                if (s_target < to_check && o_target == s_target - 1) backward++;
            }
        }
    }
    if (!s_files[7].empty()) {
        if (o_files[7].empty() && o_files[6].empty()) passed++;
        else if (!o_files[6].empty()) {
            char to_check, s_target, o_target;
            if (side) {
                to_check = *std::min_element(s_files[7].begin(), s_files[7].end());
                if (s_files[6].empty()) s_target = 7;
                else s_target = *std::min_element(s_files[6].begin(), s_files[6].end());
                o_target = *std::min_element(o_files[6].begin(), o_files[6].end());
                if (s_target > to_check) backward++;
            } else {
                to_check = *std::max_element(s_files[7].begin(), s_files[7].end());
                if (s_files[6].empty()) s_target = 0;
                else s_target = *std::min_element(s_files[6].begin(), s_files[6].end());
                o_target = *std::max_element(o_files[6].begin(), o_files[6].end());
                if (s_target < to_check) backward++;
            }
        }
    }

    // Check inner files
    for (char i = 1; i < 7; i++) {
        if (!s_files[i].empty()) {
            const vector<char> o_left = o_files[i-1], o_right = o_files[i+1];
            const vector<char> s_left = s_files[i-1], s_right = s_files[i+1];
            if (o_left.empty() && o_files[i].empty() && o_right.empty()) passed++;
            else if (!o_left.empty() || !o_right.empty()) {
                char to_check, s_target;
                if (side) {
                    to_check = *std::min_element(s_files[i].begin(), s_files[i].end());
                    if (std::find(o_left.begin(), o_left.end(), to_check+2) != o_left.end() ||
                            std::find(o_right.begin(), o_right.end(), to_check+2) != o_right.end()) {
                        if (s_left.empty()) s_target = 7;
                        else s_target = *std::min_element(s_left.begin(), s_left.end());
                        if (s_target <= to_check) {
                            backward++;
                            continue;
                        }
                        if (s_right.empty()) s_target = 7;
                        else s_target = *std::min_element(s_right.begin(), s_right.end());
                        if (s_target <= to_check) backward++;
                    }
                } else {
                    to_check = *std::max_element(s_files[i].begin(), s_files[i].end());
                    if (std::find(o_left.begin(), o_left.end(), to_check-2) != o_left.end() ||
                            std::find(o_right.begin(), o_right.end(), to_check-2) != o_right.end()) {
                        if (s_left.empty()) s_target = 0;
                        else s_target = *std::max_element(s_left.begin(), s_left.end());
                        if (s_target >= to_check) {
                            backward++;
                            continue;
                        }
                        if (s_right.empty()) s_target = 0;
                        else s_target = *std::max_element(s_right.begin(), s_right.end());
                        if (s_target >= to_check) backward++;
                    }
                }
            }
        }
    }

    return (
        -0.3 * islands +
        -0.2 * doubled +
         0.4 * passed  +
        -0.2 * backward
    );
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

    const char pawn_dir = pos.turn ? 1 : -1;

    const float mat = material(pos);
    const float pawn_struct = ((float)options.EvalPawnStruct)/100 * (pawn_structure(pos.wp, pos.bp, pos.turn)-pawn_structure(pos.bp, pos.wp, !pos.turn));

    // Endgame and middle game are for weighting categories.
    const float mg = middle_game(pawn_struct);
    const float eg = end_game(pawn_struct);
    const float p = phase(pos);
    const float score = mg*p + eg*(1-p);

    return mat + score;
}
