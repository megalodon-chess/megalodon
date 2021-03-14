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

const U64 INNER_CENTER = 103481868288ULL;
const U64 OUTER_CENTER = 66125924401152ULL;
const float INNER_COUNT = Bitboard::popcnt(INNER_CENTER);
const float OUTER_COUNT = Bitboard::popcnt(OUTER_CENTER);
const float INNER_WEIGHT = 1.0;
const float OUTER_WEIGHT = 0.4;

float material(Position);
float material_weight(Options&, int);

float center(U64, U64);
float center_weight(Options&, int);

float eval(Options&, Position, bool, U64=0, U64=0);
