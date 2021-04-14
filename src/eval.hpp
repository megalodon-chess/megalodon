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
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace Eval {
    constexpr int MIDGAME_LIM = 50;
    constexpr int ENDGAME_LIM = 20;
    constexpr U64 INNER_CENTER = 103481868288ULL;
    constexpr U64 OUTER_CENTER = 66125924401152ULL;

    void init();
    char center_dist(const char&);

    float material(const Position&);
    float total_mat(const Position&);
    float non_pawn_mat(const Position&);

    float phase(const Position&);
    float middle_game(const float&, const float&, const float&);
    float end_game(const float&, const float&, const float&);

    char center_dist(const char&);

    float pawn_structure(const U64&, const U64&);
    float space(const U64&, const U64&);
    float knights(const U64&, const U64&, const U64&, const U64&);
    float kings(const U64&, const U64&);

    char center_dist(const char&);

    char eval_end(const Position&, const U64&, const vector<Move>&);
    float eval(const Options&, const Position&, const vector<Move>&, const int&, const U64&, const bool=false);
}
