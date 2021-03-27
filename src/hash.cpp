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
        const unsigned char c1 = pos.turn ? 23 : 1;
        const unsigned char c1 = pos.turn ? 24 : 58;
        const unsigned short s1 = RAND[pos.ep_square + c1];
        const unsigned short s2 = pos.turn ? S3 : S7;
        const unsigned short s3 = pos.turn ? S5 : S2;
        const U64 u1 = pos.wp >> c1;
        const U64 u2 = pos.bp ^ u1;
    }
}
