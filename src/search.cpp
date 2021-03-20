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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


SearchInfo::SearchInfo() {
}

SearchInfo::SearchInfo(int _depth, int _seldepth, bool _is_mate, float _score, int _nodes, int _nps, int _time, Move _move) {
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
    str += " pv ";// + Bitboard::move_str(move);
    return str;
}


float moves_left(Options& options, Position pos) {
    // Estimates moves left based on material and move stack.
    //float mat_left = 0.9 * (total_mat(pos)-7);
    float abs_left = 55 - pos.move_stack.size();
    if (abs_left < 5) abs_left = 5;
    //if (mat_left < 0) mat_left = 0;

    //float final_left = (mat_left+abs_left) / 2;
    //if (final_left < 1) final_left = 1;

    return abs_left;
}

float move_time(Options& options, Position pos, float time, float inc) {
    // Calculates move time based on moves left and game evaluation.
    float moves = moves_left(options, pos);
    float time_left = time + inc*moves;
    return time_left / moves;
    //float mat = eval(options, pos, false);
    //if (!pos.turn) mat *= -1;
    //float mat_offset = mat * -0.3;

    //return (time_left/moves) + mat_offset;
}


SearchInfo search2(Options& options, Position pos, float alpha, float beta, bool root, int depth, double max_time) {
    vector<vector<Position>> nodes = {{pos}};
    Move best_move;
    int num_nodes = 1;

    while (true) {
        int curr_depth = nodes.size() - 1;
        int depth_done = true;

        if (nodes.size() < depth) {
            for (auto& node: nodes[curr_depth]) {
                if (!node.done) {
                    depth_done = false;
                    break;
                }
            }
        }

        if (depth_done) {
            Position* target_node;
            bool found = false;
            for (auto& node: nodes[curr_depth-1]) {
                if (!node.done) {
                    target_node = &node;
                    found = true;
                    break;
                }
            }

            if (found) {
                target_node->eval = target_node->turn ? MIN : MAX;
                for (auto& node: nodes[curr_depth]) {
                    bool exceeds = false;
                    if (target_node->turn && (node.eval > target_node->eval)) exceeds = true;
                    else if (!target_node->turn && (node.eval < target_node->eval)) exceeds = true;
                    if (exceeds) {
                        target_node->eval = node.eval;
                        if (curr_depth == 1) best_move = node.move_stack[node.move_stack.size()-1];
                    }
                }
                target_node->done = true;
            }
            nodes.erase(nodes.end()-1);

        } else {
            vector<Position> new_depth;
            Position target_node;
            for (auto& node: nodes[curr_depth]) {
                if (!node.done) {
                    target_node = node;
                    break;
                }
            }

            U64 o_attacks = Bitboard::attacked(target_node, !target_node.turn);
            vector<Move> moves = Bitboard::legal_moves(target_node, o_attacks);
            for (auto& move: moves) {
                Position new_node = Bitboard::push(target_node, move);
                if (curr_depth == depth-2) new_node.eval = eval(options, new_node, (moves.size()!=0), curr_depth, o_attacks);
                new_depth.push_back(new_node);
            }
            nodes.push_back(new_depth);
            num_nodes += moves.size();
        }

        if (nodes[0][0].done) break;
    }

    return SearchInfo(depth, depth, false, nodes[0][0].eval, num_nodes, 0, 0, best_move);
}

SearchInfo search(Options& options, Position pos, float alpha, float beta, bool root, int depth, double max_time) {
    U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
    vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);

    if (depth == 0 || moves.size() == 0) {
        float score = eval(options, pos, (moves.size() != 0), depth, o_attacks);
        return SearchInfo(depth, depth, false, score, 1, 0, 0, Move());
    }
    int nodes = 1;
    int best_ind = 0;
    float best_eval = pos.turn ? MIN : MAX;

    for (auto i = 0; i < moves.size(); i++) {
        Position new_pos = Bitboard::push(pos, moves[i]);
        SearchInfo result = search(options, new_pos, alpha, beta, false, depth-1, max_time);
        nodes += result.nodes;

        if (pos.turn) {
            if (result.score > best_eval) {
                best_ind = i;
                best_eval = result.score;
            }
            if (result.score > alpha) alpha = result.score;
            if (beta <= alpha) break;
        } else {
            if (result.score < best_eval) {
                best_ind = i;
                best_eval = result.score;
            }
            if (result.score < beta) beta = result.score;
            if (beta <= alpha) break;
        }
    }
    return SearchInfo(depth, depth, false, best_eval, nodes, 0, 0, moves[best_ind]);
}
