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

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "chess/board.hpp"
#include "chess/move.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;

typedef std::pair<float, Move> evalmove;


class Node {
public:
    Node();
    Node(Board, int);

    void set_inactive();
    void branch(int);

    int node_count();
    evalmove minimax(Options&);

private:
    vector<Node> _branches;
    Board _board;
    bool _active;
    int _depth;
};


class Tree {
public:
    Tree();

    void setup();
    bool active();

    void stop(Options&);
    void go_depth(Options&, int);

private:
    Node _root;
    bool _active;
};


evalmove minimax(Board, Options&, int, int);
