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

void Node::set_inactive() {
    _active = false;
    for (auto node: _branches) {
        node._active = false;
    }
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

int Node::node_count() {
    int count = 1;
    for (auto node: _branches) count += node.node_count();
    return count;
}

evalmove Node::minimax(Options& options) {
    if (_branches.size() == 0) {
        return evalmove(eval(_board, options), _board.peek());
    } else {
        int best_ind = 0;
        int best_eval = _board.turn() ? INT_MIN : INT_MAX;

        for (auto i = 0; i < _branches.size(); i++) {
            evalmove result = _branches[i].minimax(options);
            bool exceeds = false;

            if (_board.turn() && result.first > best_eval) exceeds = true;
            if (!_board.turn() && result.first < best_eval) exceeds = true;
            if (exceeds) {
                best_ind = i;
                best_eval = result.first;
            }
        }
        return evalmove(best_eval, _branches[best_ind]._board.peek());
    }
}


Tree::Tree() {
    _active = false;
}

void Tree::setup() {
    _active = true;
}

void Tree::stop(Options& options) {
    _root.set_inactive();
    Move best = _root.minimax(options).second;
    cout << "bestmove " << best.uci() << endl;
}

void Tree::go_depth(Options& options, int depth) {
    for (auto d = 0; d < depth; d++) {
        _root.branch(d);
    }
    stop(options);
}
