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
#include "options.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

vector<string> GREETINGS = {"Hello!", "Lets play!", "Are you ready for a game?"};


Position parse_pos(string str) {
    vector<string> parts = split(str, " ");
    if (parts[1] == "startpos") {
        Position pos;
        pos.wp = Bitboard::START_WP;
        pos.wn = Bitboard::START_WN;
        pos.wb = Bitboard::START_WB;
        pos.wr = Bitboard::START_WR;
        pos.wq = Bitboard::START_WQ;
        pos.wk = Bitboard::START_WK;
        pos.bp = Bitboard::START_BP;
        pos.bn = Bitboard::START_BN;
        pos.bb = Bitboard::START_BB;
        pos.br = Bitboard::START_BR;
        pos.bq = Bitboard::START_BQ;
        pos.bk = Bitboard::START_BK;

        return pos;
    }
}


void chat(Options& options, int movect) {
    if (movect == 0) {
        cout << GREETINGS[rand()%GREETINGS.size()];
    }
}


int loop() {
    string cmd;
    Options options;
    Position pos;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "option name Chat type check default false" << "\n";
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];

            if (name == "Chat") options.Chat = (value == "true");
        }

        else if (cmd == "d");
        else if (cmd == "eval");

        else if (cmd == "ucinewgame");
        else if (startswith(cmd, "position")) pos = parse_pos(cmd);
        else if (startswith(cmd, "go")) {
            chat(options, pos.move_stack.size());
        }
        else if (cmd == "stop");
    }

    return 0;
}
