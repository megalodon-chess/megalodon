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
    U64 hash(const Position& pos) {
        const unsigned char c1 = pos.turn ? 5 : 41;
        const unsigned char c2 = pos.turn ? 23 : 1;
        const unsigned char c3 = pos.turn ? 29 : 54;
        const unsigned char c4 = pos.turn ? 11 : 36;
        const unsigned short s1 = RANDS[pos.ep_square + c1];
        const unsigned short s2 = pos.turn ? S3 : S7;
        const unsigned short s3 = pos.turn ? S5 : S2;
        const U64 u1 = pos.wp >> c1;
        const U64 u2 = pos.bp ^ u1;
        const U64 u3 = pos.wn ^ pos.bq | u2;
        const U64 u4 = pos.wk ^ pos.bn | u1;
        const U64 u5 = ((U64)c1) << abs(c2-c3);
        const U64 u6 = pos.wn | pos.bb | pos.wr | pos.br;
        const U64 u7 =  ((S2<<c1) + u4) ^ ((S3<<c3) + u1) + pos.wn - pos.br + pos.wp - pos.wk;
        const U64 u8 =  ((S7<<c4) + u6) | ((S5<<c3) + u2) + pos.bb - pos.wk + pos.bp - pos.wn;
        const U64 u9 =  ((S5<<c2) + u1) ^ ((S7<<c1) + u2) + pos.wb - pos.bk + pos.wn - pos.wp;
        const U64 u10 = ((S2<<c2) + u3) | ((S3<<c4) + u6) + pos.bq - pos.wq + pos.bn - pos.bp;
        return (u7>>c1) + (u8>>c2) + (u9>>c3) + (u10>>c4);
    }
}
