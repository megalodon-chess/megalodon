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

struct SearchInfo {
    SearchInfo();
    SearchInfo(int, int, bool, float, int, int, int, Move);
    string as_string();

    int depth;
    int seldepth;
    bool is_mate_score;
    float score;
    int nodes;
    int nps;
    int time;
    Move move;
};

constexpr float MAX = 1000000;
constexpr float MIN = -1000000;

float moves_left(Options&, Position);
float move_time(Options&, Position, float, float);

SearchInfo search(Options&, Position, float, float, bool, int, double);
SearchInfo search2(Options&, Position, float, float, bool, int, double);
