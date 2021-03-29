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
#include <algorithm>
#include <string>
#include "hash.hpp"
#include "bitboard.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


namespace Hash {
    U64 piece_bits[64][12];

    void init() {
        for (auto i = 0; i < 64; i++) {
            for (auto j = 0; j < 12; j++) {
                piece_bits[i][j] = ((U64)rand()) * ((U64)rand());
            }
        }
    }

    U64 hash(const Position& pos) {
        U64 value = 0;
        for (auto i = 0; i < 64; i++) {
            if      (Bitboard::bit(pos.wp, i)) value ^= piece_bits[i][0];
            else if (Bitboard::bit(pos.wn, i)) value ^= piece_bits[i][1];
            else if (Bitboard::bit(pos.wb, i)) value ^= piece_bits[i][2];
            else if (Bitboard::bit(pos.wr, i)) value ^= piece_bits[i][3];
            else if (Bitboard::bit(pos.wq, i)) value ^= piece_bits[i][4];
            else if (Bitboard::bit(pos.wk, i)) value ^= piece_bits[i][5];
            else if (Bitboard::bit(pos.bp, i)) value ^= piece_bits[i][6];
            else if (Bitboard::bit(pos.bn, i)) value ^= piece_bits[i][7];
            else if (Bitboard::bit(pos.bb, i)) value ^= piece_bits[i][8];
            else if (Bitboard::bit(pos.br, i)) value ^= piece_bits[i][9];
            else if (Bitboard::bit(pos.bq, i)) value ^= piece_bits[i][10];
            else if (Bitboard::bit(pos.bk, i)) value ^= piece_bits[i][11];
        }
        return value;
    }
}
