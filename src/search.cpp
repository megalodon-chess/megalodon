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
#include <chrono>
#include "chess/board.hpp"
#include "eval.hpp"
#include "search.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


float get_time() {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
}


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
    if (target_depth == _depth) {
        for (auto move: _board.get_all_legal_moves()) {
            if (!_active) return;
            Board new_board = _board.copy();
            new_board.push(move);
            _branches.push_back(Node(new_board, _depth+1));
        }
    } else if (target_depth > _depth) {
        for (auto i = 0; i < _branches.size(); i++) {
            if (_depth == 0) cout << i << endl;
            if (!_active) return;
            _branches[i].branch(target_depth);
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
    _depth = 0;
    _time_start = get_time();
}

void Tree::print_info() {
    int nodes = _root.node_count();
    float elapse = get_time() - _time_start;

    cout << "info depth " << _depth << " seldepth " << _depth << " multipv 1 score cp " << (int)(_score*100) << " nodes "
        << nodes << " nps " << (int)(nodes/elapse) << " tbhits 0 time " << (int)(elapse*1000) << " pv" << endl;
}

bool Tree::active() {
    return _active;
}

void Tree::stop(Options& options) {
    _root.set_inactive();
    evalmove result = _root.minimax(options);
    _score = result.first;

    print_info();
    cout << "bestmove " << result.second.uci() << endl;
}

void Tree::go_depth(Options& options, Board board, int depth) {
    _root = Node(board, 0);
    setup();

    for (auto d = 0; d < depth; d++) {
        _depth = d;
        _root.branch(d);
    }
    stop(options);
}
