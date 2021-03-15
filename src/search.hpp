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

typedef vector<vector<vector<Position>>> Pos3D;
typedef vector<vector<Position>> Pos2D;
typedef vector<Position> Pos1D;

struct SearchInfo {
    SearchInfo();
    SearchInfo(int, int, bool, int, int, int, int, Move);
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

constexpr int MAX = 1000000;
constexpr int MIN = -1000000;

float total_mat(Position);
float moves_left(Options&, Position);
float move_time(Options&, Position, float, float);

SearchInfo bfs(Options&, Position, int);
SearchInfo dfs(Options&, Position, int, float=-1000000, float=1000000);
