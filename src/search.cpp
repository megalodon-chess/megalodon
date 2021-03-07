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


Node::Node() {
    _branches = {};
    _active = true;
}

Node::Node(Board board, int depth) {
    _board = board;
    _depth = depth;
    _branches = {};
    _active = true;
}

void Node::branch(int target_depth) {
    if (target_depth == _depth+1) {
        for (auto move: _board.get_all_legal_moves()) {
            if (!_active) return;
            Board new_board = _board.copy();
            new_board.push(move);
            Node new_node = Node(new_board, _depth+1);
            _branches.push_back(new_node);
        }
    } else if (target_depth > _depth+1) {
        for (auto node: _branches) {
            if (!_active) return;
            node.branch(target_depth);
        }
    }
}


Tree::Tree() {
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
