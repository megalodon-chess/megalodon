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


constexpr int MIDGAME_LIM = 50;
constexpr int ENDGAME_LIM = 20;
constexpr int LIM_DIFF = MIDGAME_LIM - ENDGAME_LIM;
constexpr int MAX_PHASE = 78;

float material(const Position&);
float total_mat(const Position&);
float non_pawn_mat(const Position&);

float phase(const Position&);
float middle_game(const float&);
float end_game(const float&);

float pawn_structure(const U64&, const U64&);

float eval(const Options&, const Position&, const bool&, const int&, const U64&);
