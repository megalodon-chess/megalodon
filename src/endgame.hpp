//
//  Megalodon
//  UCI chess engine
//  Copyright the Megalodon developers
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
#include "bitboard.hpp"

namespace Endgame {
    const std::vector<char> W_KQvK = {0, 0, 0, 0, 1, 0, 0, 0, 0, 0};
    const std::vector<char> B_KQvK = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};
    const std::vector<char> CORNERS = {0, 7, 56, 63};

    bool cnt_match(const std::vector<char>, const std::vector<char>);
    std::vector<char> get_cnts(const Position&);
    int eg_type(const Position&);

    Move bestmove(const Position&, const std::vector<Move>&, const int&);

    Move kqvk(const std::vector<Move>&, const Position&, const U64&, const U64&, const U64&);
}
