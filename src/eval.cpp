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

#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "bitboard.hpp"
#include "options.hpp"
#include "eval.hpp"
#include "search.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


float material(Position pos, int movect) {
    float value = 0;
    for (auto i = 0; i < 64; i++) {
        if (Bitboard::bit(pos.wp, i)) value += 1;
        else if (Bitboard::bit(pos.wn, i)) value += 3;
        else if (Bitboard::bit(pos.wb, i)) value += 3;
        else if (Bitboard::bit(pos.wr, i)) value += 5;
        else if (Bitboard::bit(pos.wq, i)) value += 9;
        else if (Bitboard::bit(pos.bp, i)) value -= 1;
        else if (Bitboard::bit(pos.bn, i)) value -= 3;
        else if (Bitboard::bit(pos.bb, i)) value -= 3;
        else if (Bitboard::bit(pos.br, i)) value -= 5;
        else if (Bitboard::bit(pos.bq, i)) value -= 9;
    }
    return value;
}


float one_pawn(vector<char> pos) {
    char x = pos[0];
    char y = pos[1];

    if (y == 0) return PAWN_R1[x];
    if (y == 1) return PAWN_R2[x];
    if (y == 2) return PAWN_R3[x];
    if (y == 3) return PAWN_R4[x];
    if (y == 4) return PAWN_R5[x];
    if (y == 5) return PAWN_R6[x];
    if (y == 6) return PAWN_R7[x];
    return PAWN_R8[x];
}

float pawns(Position pos, int movect) {
    vector<vector<char>> wpawns;
    vector<vector<char>> bpawns;
    for (char i = 0; i < 64; i++) {
        if (Bitboard::bit(pos.wp, i)) wpawns.push_back({(char)(i%8), (char)(i/8)});
        if (Bitboard::bit(pos.bp, i)) bpawns.push_back({(char)(i%8), (char)(7-i/8)});
    }

    int score;
    for (auto p: wpawns) score += one_pawn(p);
    for (auto p: wpawns) score -= one_pawn(p);

    return score / (wpawns.size()+bpawns.size());
}


float eval(Options& options, Position pos, bool moves_exist) {
    if (!moves_exist) {
        if (pos.turn) return MIN;
        else return MAX;
    }

    int movect = pos.move_stack.size();
    float mat = material(pos, movect);
    return mat;
}
