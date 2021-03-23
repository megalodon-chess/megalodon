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
    SearchInfo(int, int, bool, float, int, int, double, Move, float, float);
    string as_string();

    int depth;
    int seldepth;
    bool is_mate_score;
    float score;
    int nodes;
    int nps;
    double time;
    Move move;
    float alpha;
    float beta;
};

constexpr float MAX = 1000000;
constexpr float MIN = -1000000;

float moves_left(const Options&, const Position&);
float move_time(const Options&, const Position&, const float&, const float&);

SearchInfo search(const Options&, Position, const int&);
