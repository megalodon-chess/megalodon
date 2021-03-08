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
#include "funcs.hpp"
#include "bitboard.hpp"
#include "search.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


void loop() {
    string cmd;
    Position pos(Bitboard::START_WP, Bitboard::START_WN, Bitboard::START_WB, Bitboard::START_WR, Bitboard::START_WQ,
        Bitboard::START_WK, Bitboard::START_BP, Bitboard::START_BN, Bitboard::START_BB, Bitboard::START_BR,
        Bitboard::START_BQ, Bitboard::START_BK, true, 15, false, 0);

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];
        }

        else if (cmd == "d");
        else if (cmd == "eval");

        else if (cmd == "ucinewgame");
        else if (startswith(cmd, "position"));
        else if (startswith(cmd, "go")) {
            evalmove result = minimax(pos, 4);
        }
        else if (cmd == "stop");
    }
}
