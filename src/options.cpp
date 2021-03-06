//
//  Megalodon
//  UCI chess engine
//  Copyright Patrick Huang and Arjun Sahlot 2021
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
#include "piecemap.hpp"
#include "options.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


Options::Options() {
    UsePieceMaps = true;

    pm_loaded = false;
    pm_pawn = PieceMap();
    pm_knight = PieceMap();
    pm_bishop = PieceMap();
    pm_rook = PieceMap();
    pm_queen = PieceMap();
    pm_king = PieceMap();
}
