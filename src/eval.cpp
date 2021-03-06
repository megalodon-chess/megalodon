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
#include <fstream>
#include <vector>
#include <string>
#include "chess/board.hpp"
#include "chess/funcs.hpp"
#include "eval.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


PieceMap::PieceMap() {
    _weights = {};
    for (auto y = 0; y < 8; y++) {
        vector<float> row;
        for (auto x = 0; x < 8; x++) row.push_back(0);
        _weights.push_back(row);
    }
}

PieceMap::PieceMap(string fname) {
    std::ifstream file(fname);
    _weights = {};
    for (auto y = 0; y < 8; y++) {
        vector<float> row;
        for (auto x = 0; x < 8; x++) {
            float weight;
            file >> weight;
            row.push_back(weight);
        }
        _weights.push_back(row);
    }
}

void PieceMap::normalize(int bound) {
    int max_val = 0;
    for (auto row: _weights) {
        for (auto weight: row) {
            if (weight > max_val) max_val = weight;
        }
    }

    for (auto y = 0; y < 8; y++) {
        for (auto x = 0; x < 8; x++) {
            _weights[y][x] *= (bound/max_val); 
        }
    }
}

float PieceMap::eval(int x, int y) {
    return _weights[y][x];
}


float material(Board board) {
    float mat = 0;

    for (auto row: board.board()) {
        for (auto piece: row) {
            int value = piece_value(piece);
            mat += (piece >= 11) ? -value : value;
        }
    }

    return mat;
}

float material_weight(int movect) {
    return 1.0;
}


float piece_map(Board board) {
    return 123;
}

float piece_map_weight(int movect) {
    return 1.0;
}


float eval(Board board) {
    int movect = board.move_stack().size();

    float mat = material(board) * material_weight(movect);

    return mat;
}
