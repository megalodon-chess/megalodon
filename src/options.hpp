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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

class Options {
/*
EvalMaterial: type=spin, default=100, min=0, max=1000, weight (percent) of material eval.
EvalCenter: type=spin, default=100, min=0, max=1000, weight (percent) of center control eval.
EvalKing: type=spin, default=100, min=0, max=1000, weight (percent) of king eval.
EvalPawn: type=spin, default=100, min=0, max=1000, weight (percent) of pawn eval.
EvalKnight: type=spin, default=100, min=0, max=1000, weight (percent) of knight eval.
EvalRook: type=spin, default=100, min=0, max=1000, weight (percent) of rook eval.
EvalRook: type=spin, default=100, min=0, max=1000, weight (percent) of queen eval.
Chat: type=check, default=true, whether the engine should chat with you.
*/

public:
    Options();

    int EvalMaterial;
    int EvalCenter;
    int EvalKing;
    int EvalPawn;
    int EvalKnight;
    int EvalRook;
    int EvalQueen;
    bool Chat;
};
