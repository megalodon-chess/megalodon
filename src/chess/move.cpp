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
    _promo_piece = WP;
    _from_square = {0, 0};
    _to_square = {0, 0};
}

Move::Move(string uci) {
    _to_square = string_to_square(uci.substr(0, 2));
    _from_square = string_to_square(uci.substr(2, 2));
}

vector<int> Move::to_square() {
    return _to_square;
}

vector<int> Move::from_square() {
    return _from_square;
}

std::ostream& operator << (std::ostream& out, const Move& move) {
    out << "<Move(" << square_to_string(move._from_square) << square_to_string(move._to_square) << ">";
    return out;
}
