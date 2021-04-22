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
#include <string.h>
#include <vector>
#include <string>
#include "bitboard.hpp"
#include "endgame.hpp"
#include "eval.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

using Bitboard::popcnt;


namespace Endgame {
    const char DRAW_COUNT = 7;
    const char DRAWS[DRAW_COUNT][10] = {
    //   P  N  B  R  Q  p  n  b  r  q
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // K  v K
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // KB v K
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // K  v KB
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // KN v K
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // K  v KN
    };
    const char CHAR_BYTES = 10; // sizeof(char)*10

    bool equal(const char a1[10], const char a2[10]) {
        return memcmp(a1, a2, CHAR_BYTES) == 0;
    }

    bool is_draw(const Position& pos) {
        if (popcnt(Bitboard::get_all(pos)) > 5) return false;

        const char counts[10] = {
            popcnt(pos.wp),
            popcnt(pos.wn),
            popcnt(pos.wb),
            popcnt(pos.wr),
            popcnt(pos.wq),
            popcnt(pos.bp),
            popcnt(pos.bn),
            popcnt(pos.bb),
            popcnt(pos.br),
            popcnt(pos.bq),
        };
        for (unsigned char i = 0; i < DRAW_COUNT; i++) {
            if (equal(DRAWS[i], counts)) return true;
        }
        return false;
    }
}
