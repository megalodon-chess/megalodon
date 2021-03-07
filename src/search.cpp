//
//  Megalodon
//  UCI chess engine
//  Copyright Patrick Huang and Arjun Sahlot 2021
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
#include <climits>
#include "chess/board.hpp"
#include "eval.hpp"
#include "search.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


Tree::Tree() {
}


Node::Node() {
}

Board Node::board() {
    return _board;
}

bool Node::active() {
    return _active;
}

int Node::depth() {
    return _depth;
}

void Node::set_board(Board board) {
    _board = board;
}

void Node::set_active(bool active) {
    _active = active;
}

void Node::set_depth(int depth) {
    _depth = depth;
}

void Node::branch(int target_depth) {
}


evalmove minimax(Board board, Options& options, int depth, int max_depth) {
    if (depth == max_depth) {
        return evalmove(eval(board, options), board.peek());
    } else if (depth < max_depth) {
        vector<Move> moves = board.get_all_legal_moves();
        int best_ind = 0;
        int best_eval = board.turn() ? INT_MIN : INT_MAX;
        Move best_move;

        for (auto i = 0; i < moves.size(); i++) {
            Move move = moves[i];
            Board new_board = board.copy();
            new_board.push(move);
            evalmove result = minimax(new_board, options, depth+1, max_depth);

            bool exceeds = false;
            if (board.turn() && result.first > best_eval) exceeds = true;
            if (!board.turn() && result.first < best_eval) exceeds = true;
            if (exceeds) {
                best_ind = i;
                best_eval = result.first;
            }
        }
        return evalmove(best_eval, moves[best_ind]);
    }
}
