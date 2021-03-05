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
#include "board.hpp"
#include "constants.hpp"
#include "funcs.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


string piece_to_symbol(int piece) {
    switch (piece) {
        case WP: return "P";
        case WN: return "N";
        case WB: return "B";
        case WR: return "R";
        case WQ: return "Q";
        case WK: return "K";
        case BP: return "p";
        case BN: return "n";
        case BB: return "b";
        case BR: return "r";
        case BQ: return "q";
        case BK: return "k";
    }
}

int symbol_to_piece(string symbol) {
    switch (symbol[0]) {
        case 'P': return WP;
        case 'N': return WN;
        case 'B': return WB;
        case 'R': return WR;
        case 'Q': return WQ;
        case 'K': return WK;
        case 'p': return BP;
        case 'n': return BN;
        case 'b': return BB;
        case 'r': return BR;
        case 'q': return BQ;
        case 'k': return BK;
    }
}


int letter_to_column(char letter) {
    if (letter >= 97) {
        return letter - 97;
    } else {
        return letter - 65;
    }
}

vector<int> string_to_square(string str) {
    int col = letter_to_column(str[0]);
    int row = std::stoi(str.substr(1, 1))-1;
    return {row, col};
}

string square_to_string(vector<int> square, bool cap) {
    string column;
    if (cap) column = string(1, square[1]+65);
    else column = string(1, square[1]+97);
    return column + std::to_string(8-square[0]);
}
