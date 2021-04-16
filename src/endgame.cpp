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
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // KvK
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},   // KBvK
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // KvKB
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},   // KNvK
        {0, 0, 0, 0, 0, 0, 1, 0, 0, 0},   // KvKN
    };

    bool equal(const char a1[12], const char a2[12]) {
        for (char i = 0; i < 12; i++) {
            if (a1[i] != a2[i]) return false;
        }
        return true;
    }

    bool is_draw(const Position& pos) {
        if (popcnt(Bitboard::get_all(pos)) > 5) return false;

        const char counts[12] = {
            popcnt(pos.wp),
            popcnt(pos.wn),
            popcnt(pos.wb),
            popcnt(pos.wr),
            popcnt(pos.wq),
            popcnt(pos.wk),
            popcnt(pos.bp),
            popcnt(pos.bn),
            popcnt(pos.bb),
            popcnt(pos.br),
            popcnt(pos.bq),
            popcnt(pos.bk),
        };
        for (char i = 0; i < DRAW_COUNT; i++) {
            if (equal(DRAWS[i], counts)) return true;
        }
        return false;
    }
}
