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
#include "constants.hpp"
#include "move.hpp"
#include "funcs.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


Move::Move() {
    _promotion = false;
    _promo_piece = EM;
    _from_square = {0, 0};
    _to_square = {0, 0};
}

Move::Move(string uci) {
    _to_square = string_to_square(uci.substr(0, 2));
    _from_square = string_to_square(uci.substr(2, 2));
    if (uci.size() > 4) {
        _promo_piece = symbol_to_piece(uci.substr(4, 1));
        _promotion = true;
    } else {
        _promotion = false;
    }
}

string Move::as_string() {
    string str;
    str = "<Move(" + square_to_string(_from_square) + " " + square_to_string(_to_square) + ">";
    return str;
}

vector<int> Move::to_square() {
    return _to_square;
}

vector<int> Move::from_square() {
    return _from_square;
}

bool Move::promotion() {
    return _promotion;
}

int Move::promo_piece() {
    return _promo_piece;
}
