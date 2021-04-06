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

typedef std::pair<Move, float> MoveEval;


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


    SearchInfo dfs(const Options& options, const Position& pos, const int& depth, const int& real_depth, float alpha, float beta,
            const bool& root, const double& endtime, bool& searching) {
        // Read moves from hash table, if exists
        //const U64 idx = Hash::hash(pos) % options.hash_size;
        const U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
        //const bool computed = options.hash_table[idx].computed;
        const vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);
        // vector<MoveEval> results;
        // if (options.UseHashTable && computed && moves.size() <= Bitboard::MAX_HASH_MOVES) {
        //     const Transposition entry = options.hash_table[idx];
        //     moves = vector<Move>(entry.moves, entry.moves+entry.movecnt);
        // }

        if (depth == 0 || moves.empty()) {
            const float score = Eval::eval(options, pos, moves, depth, o_attacks);
            return SearchInfo(depth, depth, false, score, 1, 0, 0, {}, alpha, beta, true);
        }

        U64 nodes = 1;
        int best_ind = 0;
        float best_eval = pos.turn ? MIN : MAX;
        vector<Move> pv;
        bool full = true;
        //const char prune_limit = (100-options.LMRFactor) * moves.size() / 100;
        for (char i = 0; i < moves.size(); i++) {
            if (depth >= 2) {
                if (get_time() >= endtime || !searching) {
                    full = false;
                    break;
                }
            }
            //if (i > prune_limit) break;

            const Position new_pos = Bitboard::push(pos, moves[i]);
            const SearchInfo result = dfs(options, new_pos, depth-1, real_depth+1, alpha, beta, false, endtime, searching);
            nodes += result.nodes;
            //if (options.UseHashTable) results.push_back(MoveEval(moves[i], result.score));

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

        // // Sort moves
        // const char movecnt = moves.size();
        // if (options.UseHashTable && depth >= options.HashStart &&
        //         movecnt <= Bitboard::MAX_HASH_MOVES && !options.hash_table[idx].computed) {
        //     if (pos.turn) std::sort(results.begin(), results.end(), [](MoveEval x, MoveEval y){return x.second > y.second;});
        //     else          std::sort(results.begin(), results.end(), [](MoveEval x, MoveEval y){return x.second < y.second;});

        //     options.hash_table[idx].computed = true;
        //     options.hash_table[idx].movecnt = movecnt;
        //     for (char i = 0; i < movecnt; i++) {
        //         options.hash_table[idx].moves[i] = results[i].first;
        //     }
        // }

        pv.insert(pv.begin(), moves[best_ind]);
        return SearchInfo(depth, depth, false, best_eval, nodes, 0, 0, pv, alpha, beta, full);
    }

    float dfs2(const Options& options, const Position& pos, const int& depth, float& alpha, float& beta,
            const double& endtime, bool& searching, U64& nodes) {
        // Load best move from transposition table
        const U64 idx = Hash::hash(pos) % options.hash_size;
        const U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
        const bool computed = options.hash_table[idx].computed;
        vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);
        const char movecnt = moves.size();
        if (options.UseHashTable && computed) {
            moves.insert(moves.begin(), options.hash_table[idx].best);
        }

        if (depth == 0 || moves.empty()) {
            const float score = Eval::eval(options, pos, moves, depth, o_attacks);
            return score;
        }

        float best_eval = pos.turn ? MIN : MAX;
        Move best_move = moves[0];
        for (char i = 0; i < moves.size(); i++) {
            if (depth >= 2) {
                if (get_time() >= endtime || !searching) break;
            }

            const Position new_pos = Bitboard::push(pos, moves[i]);
            float new_alpha = alpha, new_beta = beta;
            const float result = dfs2(options, new_pos, depth-1, new_alpha, new_beta, endtime, searching, nodes);

            if (pos.turn) {
                if (result > best_eval) {
                    best_eval = result;
                    best_move = moves[i];
                }
                if (result > alpha) alpha = result;
                if (beta < alpha) break;
            } else {
                if (result < best_eval) {
                    best_eval = result;
                    best_move = moves[i];
                }
                if (result < beta) beta = result;
                if (beta < alpha) break;
            }
        }
        nodes += movecnt;

        // Sort moves
        if (options.UseHashTable && !computed && depth >= 2) {
            options.hash_table[idx].computed = true;
            options.hash_table[idx].best = best_move;
        }

        return best_eval;
    }

    SearchInfo search(const Options& options, const Position& pos, const int& depth, const double& movetime, bool& searching) {
        if (options.QuickMove) {
            const vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
            if (moves.size() == 1) return SearchInfo(1, 1, false, 0, 1, 0, 0, {moves[0]}, 0, 0, true);
            // TODO move when one of the next moves is mate
        }

        const int eg = Endgame::eg_type(pos);
        if (options.UseEndgame && eg != 0) {
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

            SearchInfo curr_result = dfs(options, pos, d, 0, alpha, beta, true, end, searching);
            const double elapse = get_time() - start;

            if (d >= options.ABPassStart) {
                alpha = result.alpha - options.ABPassMargin/100;
                beta = result.beta + options.ABPassMargin/100;
            }

            curr_result.time = elapse;
            curr_result.nps = curr_result.nodes / (elapse+0.001);
            if (curr_result.full) {
                if (!pos.turn) curr_result.score *= -1;   // UCI uses relative score, and Megalodon uses absolute score.
                cout << curr_result.as_string(options.PrintPv) << endl;
                result = curr_result;
            }
        }

        return result;
    }

    SearchInfo search2(const Options& options, const Position& pos, const int& depth, const double& movetime, bool& searching) {
        const double start = get_time();
        const double end = start + movetime;
        SearchInfo final_result;

        for (char d = 2; d <= depth; d++) {
            if (!searching) break;
            if (get_time() >= end) break;

            U64 nodes = 0;
            int best_ind = 0;
            float best_eval = pos.turn ? MIN : MAX;
            float alpha = MIN, beta = MAX;
            bool full = true;
            const vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));

            for (auto i = 0; i < moves.size(); i++) {
                if (get_time() >= end || !searching) {
                    full = false;
                    break;
                }

                const Position new_pos = Bitboard::push(pos, moves[i]);
                const float result = dfs2(options, new_pos, d-1, alpha, beta, end, searching, nodes);
                alpha -= 1;
                beta += 1;

                if (pos.turn) {
                    if (result > best_eval) {
                        best_eval = result;
                        best_ind = i;
                    }
                } else {
                    if (result < best_eval) {
                        best_eval = result;
                        best_ind = i;
                    }
                }
            }

            if (full) {
                Move move = moves[best_ind];
                const double elapse = get_time() - start;
                SearchInfo result = SearchInfo(d, d, false, best_eval, nodes, nodes/elapse, elapse, {move}, 0, 0, true);
                cout << result.as_string(options.PrintPv) << endl;
                final_result = result;
            }
        }

        return final_result;
    }
}
