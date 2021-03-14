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
    float mat = eval(options, pos, false, 0, 0);
    if (!pos.turn) mat *= -1;
    float mat_offset = mat * -0.3;

    return (time_left/moves) + mat_offset;
}


SearchInfo bfs_minimax(Pos3D& tree, int total_depth) {
    // Assigns evaluations based on minimax and returns best move from root.
    // todo alpha beta pruning

    for (auto d = total_depth-2; d >= 1; d--) {
        int nodect = 0;
        for (auto& group: tree[d]) {
            for (auto& node: group) {
                float best_eval = node.turn ? -1000000 : 1000000;

                Pos1D branches = tree[d+1][nodect];
                if (branches.size() == 0) continue;
                for (auto branch: branches) {
                    bool exceeds = false;
                    if (node.turn && (branch.eval > best_eval)) exceeds = true;
                    if (!node.turn && (branch.eval < best_eval)) exceeds = true;
                    if (exceeds) {
                        best_eval = branch.eval;
                    }
                }
                node.eval = best_eval;
                nodect++;
            }
        }
    }

    Position pos = tree[0][0][0];
    float best_eval = pos.turn ? -1000000 : 1000000;
    Move best_move;
    for (auto branch: flatten(tree[1])) {
        bool exceeds = false;
        if (pos.turn && (branch.eval > best_eval)) exceeds = true;
        if (!pos.turn && (branch.eval < best_eval)) exceeds = true;
        if (exceeds) {
            best_eval = branch.eval;
            best_move = branch.move_stack[branch.move_stack.size()-1];
        }
    }

    return SearchInfo(0, 0, false, best_eval, 0, 0, 0, best_move);
}

void bfs_rec_remove(Pos3D& tree, int depth, int ind) {
    // Removes target node and all its children.
    int curr_ind = 0;
    for (auto& group: tree[depth]) {
        int new_ind = curr_ind + group.size();
        if (ind > curr_ind && ind < new_ind) {
            group[ind-curr_ind].active = false;
            if (depth+1 < tree.size()) {
                for (auto i = 0; i < ind + tree[depth+1][ind].size(); i++) {
                    bfs_rec_remove(tree, depth+1, i);
                }
            }
            break;
        }
        curr_ind = new_ind;
    }
}

void bfs_prune(Pos3D& tree, int depth) {
    Pos1D nodes = flatten(tree[depth]);
    if (nodes.size() < 15) return;

    vector<float> evals;
    bool turn = tree[0][0][0].turn;
    for (auto node: nodes) evals.push_back(node.eval);
    std::sort(evals.begin(), evals.end());

    int threshold = turn ? evals.size()*0.75 : evals.size()*0.25;
    for (auto i = 0; i < nodes.size(); i++) {
        Position node = nodes[i];
        bool prune_curr = false;
        if (turn && (node.eval < evals[threshold])) prune_curr = true;
        if (!turn && (node.eval > evals[threshold])) prune_curr = true;
        if (prune_curr) bfs_rec_remove(tree, depth, i);
    }
}

SearchInfo bfs(Options& options, Position pos, int total_depth) {
    pos.eval = eval(options, pos, false, 0, 0);
    Pos3D* tree = new Pos3D({{{pos}}});
    SearchInfo result;
    int depth = 1;
    int num_nodes = 1;
    int next_print = 0;
    double start = get_time();

    // Tree generation and bad branch pruning (todo)
    while (true) {
        Pos2D* curr_depth = new Pos2D;

        for (auto node: flatten((*tree)[depth-1])) {
            if (!node.active) continue;

            Pos1D* group = new Pos1D;
            U64 attacks = Bitboard::attacked(node, node.turn);
            U64 o_attacks = Bitboard::attacked(node, !node.turn);

            for (auto move: Bitboard::legal_moves(node, o_attacks)) {
                Position new_pos = Bitboard::push(node, move);
                if (new_pos.turn) new_pos.eval = eval(options, new_pos, true, attacks, o_attacks);
                else new_pos.eval = eval(options, new_pos, true, o_attacks, attacks);
                (*group).push_back(new_pos);
            }
            (*curr_depth).push_back(*group);
            num_nodes += (*group).size();
            delete group;

            if (num_nodes > next_print) {
                double elapse = get_time() - start + 0.001;  // Add 0.001 to prevent divide by 0.
                cout << SearchInfo(depth, depth, false, result.score, num_nodes, num_nodes/elapse, elapse*1000, result.move).as_string() << endl;
                next_print += options.InfoInc * 1000;
            }
        }
        (*tree).push_back(*curr_depth);
        delete curr_depth;

        //if (depth >= 3) {
        //    result = bfs_minimax(*tree, depth);
        //    bfs_prune(*tree, depth-2);
        //}
        double elapse = get_time() - start + 0.001;  // Add 0.001 to prevent divide by 0.
        cout << SearchInfo(depth, depth, false, result.score, num_nodes, num_nodes/elapse, elapse*1000, result.move).as_string() << endl;

        depth++;
        if (depth == total_depth) break;
    }

    result = bfs_minimax(*tree, total_depth);
    delete tree;
    return SearchInfo(depth, depth, false, result.score, num_nodes, 0, 0, result.move);
}


SearchInfo dfs(Options& options, Position pos, int depth) {
    U64 attacks = Bitboard::attacked(pos, pos.turn);
    U64 o_attacks = Bitboard::attacked(pos, !pos.turn);
    vector<Move> moves = Bitboard::legal_moves(pos, o_attacks);

    if (depth == 0 || moves.size() == 0) {
        float score;
        if (pos.turn) score = eval(options, pos, true, attacks, o_attacks);
        else score = eval(options, pos, true, o_attacks, attacks);
        return SearchInfo(depth, depth, false, score, 1, 0, 0, Move());
    } else {
        int nodes = 1;
        int best_ind = 0;
        float best_eval = pos.turn ? -1000000 : 1000000;

        for (auto i = 0; i < moves.size(); i++) {
            Position new_pos = Bitboard::push(pos, moves[i]);;
            SearchInfo result = dfs(options, new_pos, depth-1);
            nodes += result.nodes;

            bool exceeds = false;
            if (pos.turn && (result.score > best_eval)) exceeds = true;
            if (!pos.turn && (result.score < best_eval)) exceeds = true;
            if (exceeds) {
                best_ind = i;
                best_eval = result.score;
            }
        }
        return SearchInfo(depth, depth, false, best_eval, nodes, 0, 0, moves[best_ind]);
    }
}
