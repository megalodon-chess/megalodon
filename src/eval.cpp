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


float piece_map(Board board, Options& options) {
    if (!options.pm_loaded) {
        options.pm_pawn.set_weights("pmaps/pawn");
        options.pm_knight.set_weights("pmaps/knight");
        options.pm_bishop.set_weights("pmaps/bishop");
        options.pm_rook.set_weights("pmaps/rook");
        options.pm_queen.set_weights("pmaps/queen");
        options.pm_king.set_weights("pmaps/king");
        options.pm_loaded = true;
    }

    return 123;
}

float piece_map_weight(int movect) {
    return 1.0;
}


float eval(Board board, Options& options) {
    int movect = board.move_stack().size();

    float mat = material(board) * material_weight(movect);
    float pmaps;
    if (options.UsePieceMaps) pmaps = piece_map(board, options) * piece_map_weight(movect);
    else pmaps = 0;

    return mat + pmaps;
}
