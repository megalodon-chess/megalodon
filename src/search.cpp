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
#include <algorithm>
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

using Bitboard::popcnt;


SearchInfo::SearchInfo() {
}

SearchInfo::SearchInfo(const int& _depth, const int& _seldepth, const float& _score, const U64& _nodes,
        const vector<Move>& _pv, const float& _alpha, const float& _beta, const bool& _full) {
    depth = _depth;
    seldepth = _seldepth;
    score = _score;
    nodes = _nodes;
    pv = _pv;
    alpha = _alpha;
    beta = _beta;
    full = _full;
}

string SearchInfo::as_string() {
    string str = "";
    str += "info depth " + std::to_string(depth) + " seldepth " + std::to_string(seldepth);
    str += " multipv 1 score ";
    if (is_mate()) {
        str += "mate ";
        unsigned char moves;
        if (score < 0) moves = (score-Search::MIN+1) / 2;
        else           moves = (Search::MAX-score+1) / 2;
        str += std::to_string(moves);
    } else {
        str += "cp ";
        str += std::to_string((int)(100*score));
    }
    str += " nodes " + std::to_string(nodes) + " nps " + std::to_string(nps);
    str += " hashfull " + std::to_string(hashfull);
    str += " tbhits 0 time " + std::to_string((int)(1000*time));
    str += " pv ";
    for (const auto& move: pv) str += Bitboard::move_str(move) + " ";
    return str;
}

bool SearchInfo::is_mate() {
    return !(Search::MATE_BOUND_MIN <= score && score <= Search::MATE_BOUND_MAX);
}


namespace Search {
    float move_time(const Options& options, const Position& pos, const float& time, const float& inc) {
        const int moves = std::max(55-pos.move_cnt, 5);
        const float time_left = time + inc*moves;
        const float move_time = time_left / moves;
        return std::min(move_time, time/2);
    }

    SearchInfo dfs(Options& options, const Position& pos, const double& endtime, bool& searching,
            const int& depth, const int& real_depth, float alpha, float beta) {
        const U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
        vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);

        // Return eval if depth == 0
        if (depth == 0 || moves.empty()) {
            const float score = Eval::eval(options, pos, moves, real_depth, o_attacks);
            return SearchInfo(depth, depth, score, 1, {}, alpha, beta, true);
        }

        // Read from hash table
        const U64 hash = Hash::hash(pos);
        const U64 idx = hash % options.hash_size;
        Transposition& entry = options.hash_table[idx];
        const Move best(entry.from&63, entry.to&63, entry.to>>6, entry.from>>6);
        const bool match = (
            (entry.depth    >  0)            &&
            (entry.wp       == pos.wp)       &&
            (entry.wn       == pos.wn)       &&
            (entry.wb       == pos.wb)       &&
            (entry.wr       == pos.wr)       &&
            (entry.wq       == pos.wq)       &&
            (entry.wk       == pos.wk)       &&
            (entry.bp       == pos.bp)       &&
            (entry.bn       == pos.bn)       &&
            (entry.bb       == pos.bb)       &&
            (entry.br       == pos.br)       &&
            (entry.bq       == pos.bq)       &&
            (entry.bk       == pos.bk)       &&
            (entry.turn     == pos.turn)     &&
            (entry.castling == pos.castling) &&
            (entry.ep       == pos.ep)
        );
        if (match) {
            if ((entry.depth >= depth) && (real_depth >= 1)) return SearchInfo(depth, entry.depth, entry.eval, 1, {best}, alpha, beta, true);
            if (entry.depth > 0) moves.insert(moves.begin(), best);
        }

        // Search on all child nodes
        U64 nodes = 1;
        vector<Move> pv;
        Move best_move;
        float best_eval = pos.turn ? MIN : MAX;
        bool full = true;
        int movecnt = 0;
        char seldepth = depth;
        for (const auto& move: moves) {
            if (depth >= 3) {
                if ((get_time() >= endtime) || !searching) {
                    full = false;
                    break;
                }
            }
            if ((movecnt != 0) && (entry.depth > 0) && match) {  // Don't search best move twice
                if ((best.from == move.from) && (best.to == move.to) && (best.is_promo == move.is_promo) &&
                    (best.promo == move.promo)) continue;
            }
            movecnt++;

            const Position new_pos = Bitboard::push(pos, move);
            const SearchInfo result = dfs(options, new_pos, endtime, searching, depth-1, real_depth+1, alpha, beta);
            if ((result.seldepth+1) > seldepth) seldepth = result.seldepth + 1;
            nodes += result.nodes;

            if ((real_depth == 0) && (depth >= 6)) {
                cout << "info depth " << depth << " currmove " << Bitboard::move_str(move) << " currmovenumber " << movecnt << endl;
            }

            if (pos.turn) {
                if (result.score > best_eval) {
                    best_move = move;
                    best_eval = result.score;
                    pv = result.pv;
                }
                if (result.score > alpha) alpha = result.score;
            } else {
                if (result.score < best_eval) {
                    best_move = move;
                    best_eval = result.score;
                    pv = result.pv;
                }
                if (result.score < beta) beta = result.score;
            }
            if (beta < alpha) break;
        }
        pv.insert(pv.begin(), best_move);

        // Write result into hash table
        if (full && (depth >= 2) && ((depth > entry.depth) || (!match))) {
            if (entry.depth == 0) options.hash_filled++;

            entry.from = best_move.from + (best_move.promo<<6);
            entry.to = best_move.to + (best_move.is_promo<<6);
            entry.depth = depth;
            entry.eval = best_eval;

            entry.wp = pos.wp;
            entry.wn = pos.wn;
            entry.wb = pos.wb;
            entry.wr = pos.wr;
            entry.wq = pos.wq;
            entry.wk = pos.wk;
            entry.bp = pos.bp;
            entry.bn = pos.bn;
            entry.bb = pos.bb;
            entry.br = pos.br;
            entry.bq = pos.bq;
            entry.bk = pos.bk;
            entry.turn = pos.turn;
            entry.castling = pos.castling;
            entry.ep = pos.ep;
        }

        if      (best_eval < MATE_BOUND_MIN) best_eval++;
        else if (best_eval > MATE_BOUND_MAX) best_eval--;
        return SearchInfo(depth, seldepth, best_eval, nodes, pv, alpha, beta, full);
    }

    SearchInfo search(Options& options, const Position& pos, const int& total_depth, const double& movetime,
            const bool& infinite, bool& searching) {
        const double start = get_time();
        const double endtime = start + movetime;

        SearchInfo result;
        U64 nodes = 0;
        for (auto depth = 1; depth <= total_depth; depth++) {
            SearchInfo curr_result = dfs(options, pos, endtime, searching, depth, 0, MIN, MAX);
            nodes += curr_result.nodes;

            const double elapse = get_time() - start;
            if (curr_result.full) {
                curr_result.time = elapse;
                curr_result.nodes = nodes;
                curr_result.nps = nodes / (elapse+0.001);
                curr_result.hashfull = 1000 * options.hash_filled / options.hash_size;
                if (!pos.turn) curr_result.score *= -1;
                cout << curr_result.as_string() << endl;

                result = curr_result;
            }

            if (!searching || (elapse >= movetime)) break;
        }

        return result;
    }
}
