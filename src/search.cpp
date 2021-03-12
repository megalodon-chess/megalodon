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
    str += " score ";
    str += (is_mate_score ? "mate" : "cp");
    str += " " + std::to_string(score);
    str += " nodes " + std::to_string(nodes) + " nps " + std::to_string(nps);
    return str;
}

/*
* Experimental search algorithm, to be completed in a later version.

SearchInfo search(Position pos) {
    vector<vector<vector<Position>>> tree = {{{pos}}};
    int depth = 1;
    int num_nodes = 1;

    while (true) {
        vector<vector<Position>> curr_depth;
        for (auto node: flatten(tree[depth-1])) {
            vector<Position> group;
        }
        depth++;
        if (depth == 4) break;
    }
}
*/

SearchInfo search(Options& options, Position pos, int depth) {
    if (depth == 0) {
        return SearchInfo(depth, depth, false, eval(options, pos), 0, 0, 0, Move());
    } else {
        U64 attacks = Bitboard::attacked(pos, pos.turn);
        vector<Move> moves = Bitboard::legal_moves(pos, attacks);
        int best_ind = 0;
        int best_eval = pos.turn ? INT_MIN : INT_MAX;
        Move best_move;

        for (auto i = 0; i < moves.size(); i++) {
            Position new_pos = Bitboard::copy(pos);
            new_pos = Bitboard::push(new_pos, moves[i]);
            SearchInfo result = search(options, new_pos, depth-1);

            bool exceeds = false;
            if (pos.turn && result.score > best_eval) exceeds = true;
            if (!pos.turn && result.score < best_eval) exceeds = true;
            if (exceeds) {
                best_ind = i;
                best_eval = result.score;
            }
        }
        return SearchInfo(depth, depth, false, best_eval, 0, 0, 0, moves[best_ind]);
    }
}
