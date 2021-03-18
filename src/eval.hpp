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

constexpr U64 IN_CENT = 103481868288ULL;
constexpr U64 OUT_CENT = 66125924401152ULL;
const float IN_CNT = Bitboard::popcnt(IN_CENT);
const float OUT_CNT = Bitboard::popcnt(OUT_CENT);

float material(Position);
float total_mat(Position);

float king(Options&, char, Location, U64, U64);
float pawns(Options&, U64, bool);
float knights(Options&, U64);
float rooks(Options&, U64);

float center_control(Options&, Position, int);

float eval(Options&, Position, bool, int);
