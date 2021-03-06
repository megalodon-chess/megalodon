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
#include "chess/constants.hpp"
#include "chess/board.hpp"
#include "piecemap.hpp"

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
    set_weights(fname);
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

void PieceMap::set_weights(string fname) {
    cout << "info string loading piece map from " << fname << "..." << std::flush;

    std::ifstream file(fname);
    if (!file.good()) {
        cout << "\ninfo string failed to load piece map from " << fname << ". Aborting." << endl;
        std::exit(1);
    }
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

    cout << "finished" << endl;
}

void PieceMap::normalize(float bound) {
    float max_val = 0;
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

float PieceMap::eval_square(int x, int y) {
    return _weights[y][x];
}

float PieceMap::eval(Board board, int piece) {
    vector<vector<int>> pos = board.board();
    bool black = (piece >= BP);
    int count = 0;
    float total_eval = 0;

    for (auto y = 0; y < 8; y++) {
        for (auto x = 0; x < 8; x++) {
            int curr_piece = pos[y][x];
            if (curr_piece == piece) {
                total_eval += black ? _weights[7-y][x] : _weights[y][x];
                count++;
            }
        }
    }

    return total_eval / count;
}
