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

unsigned int hash(const Position& pos) {
    return (
        (((pos.wp | pos.bp | pos.wk | pos.bk | pos.wq | pos.bq)<<3) & ((pos.wn | pos.wb | pos.bn | pos.bb)<<7)) ^
        (((pos.wp | pos.bp | pos.wr | pos.br | pos.wk | pos.bk)<<2) & ((pos.wn | pos.bn | pos.wq | pos.bq)>>3)) ^
        (((pos.wr | pos.bq | pos.wn | pos.bb | pos.wp | pos.br)>>5) & ((pos.wq | pos.bk | pos.wp | pos.bb)>>1)) |
        (((pos.wq | pos.bp | pos.wn | pos.bq | pos.wk | pos.bp)>>9) & ((pos.wp | pos.br | pos.wn | pos.bb)<<8)) ^
        (((pos.wn | pos.wq | pos.bq | pos.wq | pos.wk | pos.bn)<<9) & ((pos.wb | pos.wq | pos.bk | pos.br)<<2)) |
        (((pos.wb | pos.wp | pos.bn | pos.bp | pos.wb | pos.bn)<<8) & ((pos.wp | pos.bp | pos.wq | pos.bq)>>2)) ^
        (((pos.wq | pos.bp | pos.wr | pos.br | pos.wn | pos.bq)>>1) & ((pos.wp | pos.br | pos.wb | pos.wb)>>4)) ^
        (((pos.wk | pos.wn | pos.bq | pos.wp | pos.bk | pos.br)>>3) & ((pos.wb | pos.br | pos.bn | pos.bq)<<6))
    ) - (pos.castling*(pos.ep_square<<(pos.turn ? 1 : 9)));
}
