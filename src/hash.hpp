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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace Hash {
    constexpr unsigned short S2 = 41421;  // First 5 digits of sqrt(2) % 65536
    constexpr unsigned short S3 =  7769;  // First 5 digits of sqrt(3) % 65536
    constexpr unsigned short S5 = 23606;  // First 5 digits of sqrt(5) % 65536
    constexpr unsigned short S7 = 64575;  // First 5 digits of sqrt(7) % 65536

    U64 hash(const Position&);
}
