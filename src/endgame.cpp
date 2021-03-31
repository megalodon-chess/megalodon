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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


namespace Endgame {
    bool cnt_match(const char c1[10], const char c2[10]) {
        for (char i = 0; i < 10; i++) {
            if (c1[i] != c2[i]) return false;
        }
        return true;
    }

    int eg_type(const Position& pos) {
        const char wpc = Bitboard::popcnt(pos.wp);
        const char wnc = Bitboard::popcnt(pos.wn);
        const char wbc = Bitboard::popcnt(pos.wb);
        const char wrc = Bitboard::popcnt(pos.wr);
        const char wqc = Bitboard::popcnt(pos.wq);
        const char bpc = Bitboard::popcnt(pos.bp);
        const char bnc = Bitboard::popcnt(pos.bn);
        const char bbc = Bitboard::popcnt(pos.bb);
        const char brc = Bitboard::popcnt(pos.br);
        const char bqc = Bitboard::popcnt(pos.bq);
        const char counts[10] = {wpc, wnc, wbc, wrc, wqc, bpc, bnc, bbc, brc, bqc};

        if (pos.turn && cnt_match(W_KQvK, counts)) {
            return 1;
        } else if (!pos.turn && cnt_match(B_KQvK, counts)) {
            return 1;
        }

        return 0;
    }
}
