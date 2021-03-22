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
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


Options::Options() {
    EvalMaterial = 100;
    EvalMaterial = 100;
    EvalCenter = 100;
    EvalKing = 100;
    EvalPawn = 100;
    EvalKnight = 100;
    EvalRook = 100;
    EvalQueen = 100;
    Chat = false;
}
