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

const vector<float> PAWN_R1 = {0,  1,    0.4,  0,    0,    0,    0,  0};
const vector<float> PAWN_R2 = {0,  1,    0.1,  0,    0,    0,    0,  0};
const vector<float> PAWN_R3 = {0,  0.8,  0.6,  0.3,  0,    0,    0,  0};
const vector<float> PAWN_R4 = {0,  0.3,  0.8,  1.5,  1.4,  0.5,  0,  0};
const vector<float> PAWN_R5 = {0,  0.3,  0.8,  1.5,  1.4,  0.5,  0,  0};
const vector<float> PAWN_R6 = {0,  0.8,  0.6,  0.3,  0,    0,    0,  0};
const vector<float> PAWN_R7 = {0,  1,    0.1,  0,    0,    0,    0,  0};
const vector<float> PAWN_R8 = {0,  1,    0.4,  0,    0,    0,    0,  0};

float material(Position, int);
float pawn(Position, int);
float eval(Options&, Position, bool);
