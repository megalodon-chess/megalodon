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
#include "bitboard.hpp"
#include "search.hpp"
#include "eval.hpp"
#include "options.hpp"
#include "utils.hpp"
#include "hash.hpp"
#include "endgame.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


SearchInfo::SearchInfo() {
}

SearchInfo::SearchInfo(int _depth, int _seldepth, bool _is_mate, float _score, U64 _nodes, int _nps,
        double _time, vector<Move> _pv, float _alpha, float _beta, bool _full) {
    depth = _depth;
    seldepth = _seldepth;
    is_mate_score = !(Search::MIN+20 <= score && score <= Search::MAX-20);
    score = _score;
    nodes = _nodes;
    nps = _nps;
    time = _time;
    pv = _pv;
    alpha = _alpha;
    beta = _beta;
    full = _full;
}

string SearchInfo::as_string(bool PrintPv) {
    string str = "";
    str += "info depth " + std::to_string(depth) + " seldepth " + std::to_string(seldepth);
    str += " multipv 1 score ";
    if (is_mate_score) {
        str += "mate ";
        str += std::to_string((int)(Search::MAX-abs(score)+1));
    } else {
        str += "cp ";
        str += std::to_string((int)(100*score));
    }
    str += " nodes " + std::to_string(nodes) + " nps " + std::to_string(nps);
    str += " tbhits 0 time " + std::to_string((int)(1000*time));
    str += " pv ";
    if (PrintPv) {
        for (const auto& move: pv) str += Bitboard::move_str(move) + " ";
    }
    return str;
}


namespace Search {
    float moves_left(const Options& options, const Position& pos) {
        // Estimates moves left based on material and move stack.
        //float mat_left = 0.9 * (total_mat(pos)-7);
        float abs_left = 55 - pos.move_cnt;
        if (abs_left < 5) abs_left = 5;
        //if (mat_left < 0) mat_left = 0;

        //float final_left = (mat_left+abs_left) / 2;
        //if (final_left < 1) final_left = 1;

        return abs_left;
    }

    float move_time(const Options& options, const Position& pos, const float& time, const float& inc) {
        // Calculates move time based on moves left and game evaluation.
        float moves = moves_left(options, pos);
        float time_left = time + inc*moves;
        return time_left / moves;
        //float mat = eval(options, pos, false);
        //if (!pos.turn) mat *= -1;
        //float mat_offset = mat * -0.3;

        //return (time_left/moves) + mat_offset;
    }


    SearchInfo dfs(const Options& options, const Position& pos, const int& depth, float alpha, float beta,
            const bool& root, const double& endtime, bool& searching) {
        U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
        vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);

        if (depth == 0 || moves.empty()) {
            // const int idx = options.UseHashTable ? (Hash::hash(pos) % options.hash_size) : 0;
            // float score;
            // if (options.UseHashTable && options.hash_evaled[idx]) {
            //     score = options.hash_evals[idx];
            // } else {
            //     score = eval(options, pos, moves, depth, o_attacks);
            //     options.hash_evaled[idx] = true;
            //     options.hash_evals[idx] = score;
            // }
            const float score = Eval::eval(options, pos, moves, depth, o_attacks);
            return SearchInfo(depth, depth, false, score, 1, 0, 0, {}, alpha, beta, true);
        }

        U64 nodes = 1;
        int best_ind = 0;
        float best_eval = pos.turn ? MIN : MAX;
        vector<Move> pv;
        bool full = true;
        for (auto i = 0; i < moves.size(); i++) {
            if (depth >= 2) {
                if (get_time() >= endtime || !searching) {
                    full = false;
                    break;
                }
            }

            const Position new_pos = Bitboard::push(pos, moves[i]);
            const SearchInfo result = dfs(options, new_pos, depth-1, alpha, beta, false, endtime, searching);
            nodes += result.nodes;

            if (root && options.PrintCurrMove && (depth >= 5)) {
                cout << "info depth " << depth << " currmove " << Bitboard::move_str(moves[i])
                    << " currmovenumber " << i+1 << endl;
            }

            if (pos.turn) {
                if (result.score > best_eval) {
                    best_ind = i;
                    best_eval = result.score;
                    pv = result.pv;
                }
                if (result.score > alpha) alpha = result.score;
                if (beta < alpha) break;
            } else {
                if (result.score < best_eval) {
                    best_ind = i;
                    best_eval = result.score;
                    pv = result.pv;
                }
                if (result.score < beta) beta = result.score;
                if (beta < alpha) break;
            }
        }
        pv.insert(pv.begin(), moves[best_ind]);
        return SearchInfo(depth, depth, false, best_eval, nodes, 0, 0, pv, alpha, beta, full);
    }

    SearchInfo search(const Options& options, const Position& pos, const int& depth, const double& movetime, bool& searching) {
        const int eg = Endgame::eg_type(pos);
        if (eg != 0) {
            const vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
            const Move best_move = Endgame::bestmove(pos, moves, eg);
            return SearchInfo(1, 1, false, pos.turn ? MAX : MIN, moves.size(), 0, 0, {best_move}, 0, 0, true);
        }

        SearchInfo result;
        float alpha = MIN, beta = MAX;
        const double start = get_time();
        const double end = start + movetime;

        for (auto d = 1; d <= depth; d++) {
            if (!searching) break;
            if (get_time() >= end) break;

            SearchInfo curr_result = dfs(options, pos, d, alpha, beta, true, end, searching);
            double elapse = get_time() - start;

            if (d >= options.ABPassStart) {
                alpha = result.alpha - options.ABPassMargin/100;
                beta = result.beta + options.ABPassMargin/100;
            }

            curr_result.time = elapse;
            curr_result.nps = curr_result.nodes / (elapse+0.001);
            if (curr_result.full) cout << curr_result.as_string(options.PrintPv) << endl;
            if (curr_result.full) result = curr_result;
        }

        return result;
    }
}
