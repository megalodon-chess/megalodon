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
#include <climits>
#include <algorithm>
#include "bitboard.hpp"
#include "utils.hpp"
#include "search.hpp"
#include "eval.hpp"
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


SearchInfo::SearchInfo() {
}

SearchInfo::SearchInfo(int _depth, int _seldepth, bool _is_mate, int _score, int _nodes, int _nps, int _time, Move _move) {
    depth = _depth;
    seldepth = _seldepth;
    is_mate_score = _is_mate;
    score = _score;
    nodes = _nodes;
    nps = _nps;
    time = _time;
    move = _move;
}

string SearchInfo::as_string() {
    string str = "";
    str += "info depth " + std::to_string(depth) + " seldepth " + std::to_string(seldepth);
    str += " multipv 1 score ";
    str += (is_mate_score ? "mate" : "cp");
    str += " " + std::to_string((is_mate_score ? (int)score : (int)(100*score)));
    str += " nodes " + std::to_string(nodes) + " nps " + std::to_string(nps);
    str += " tbhits 0 time " + std::to_string(time);
    str += " pv " + Bitboard::move_str(move);
    return str;
}


float total_mat(Position pos) {
    float value = 0;
    for (auto i = 0; i < 64; i++) {
        if (Bitboard::bit(pos.wp, i)) value += 1;
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

float moves_left(Options& options, Position pos) {
    // Estimates moves left based on material and move stack.
    float mat_left = 0.9 * (total_mat(pos)-7);
    float abs_left = 65 - pos.move_stack.size();
    if (abs_left < 0) abs_left = 0;
    if (mat_left < 0) mat_left = 0;

    float final_left = (mat_left+abs_left) / 2;
    if (final_left < 1) final_left = 1;

    return final_left;
}

float move_time(Options& options, Position pos, float time, float inc) {
    // Calculates move time based on moves left and game evaluation.
    float moves = moves_left(options, pos);
    float time_left = time + inc*moves;
    float mat = eval(options, pos, false, 0, 0, false);
    if (!pos.turn) mat *= -1;
    float mat_offset = mat * -0.3;

    return (time_left/moves) + mat_offset;
}
