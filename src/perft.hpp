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
#include "consts.hpp"

namespace Perft {
    long long movegen(const Position&, const int&);
    double hash_perft(const Position&, const int&);
    double eval_perft(const Options&, const Position&, const int&);
    double push_perft(const Position&, const int&);
}
