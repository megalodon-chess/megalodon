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
#include "options.hpp"

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

string PieceMap::as_string() {
    string str;
    str += " " + BOARD_OUTROW + "\n";
    for (auto i = 0; i < 8; i++) {
        str += BOARD_OUTCOL;
        for (auto piece: _weights[i]) {
            str += std::to_string(piece) + BOARD_OUTCOL;
        }
        str += std::to_string(8-i) + "\n";
        str += " " + BOARD_OUTROW + "\n";
    }
    str += "   ";
    for (auto i: "abcdefgh") str += string(1, i) + "   ";

    return str;
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


float piece_map(Board board, Options options) {
    if (!options.pm_loaded) {
        options.pmaps["pawn"] = PieceMap("pmaps/pawn");
        options.pmaps["knight"] = PieceMap("pmaps/knight");
        options.pmaps["bishop"] = PieceMap("pmaps/bishop");
        options.pmaps["rook"] = PieceMap("pmaps/rook");
        options.pmaps["queen"] = PieceMap("pmaps/queen");
        options.pmaps["king"] = PieceMap("pmaps/king");
        options.pm_loaded = true;
    }

    return 123;
}

float piece_map_weight(int movect) {
    return 1.0;
}


float eval(Board board, Options options) {
    int movect = board.move_stack().size();

    float mat = material(board) * material_weight(movect);
    float pmaps;
    if (options.UsePieceMaps) pmaps = piece_map(board, options) * piece_map_weight(movect);
    else pmaps = 0;

    return mat + pmaps;
}
