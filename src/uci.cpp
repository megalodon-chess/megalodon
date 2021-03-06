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
#include "chess/board.hpp"
#include "chess/funcs.hpp"
#include "eval.hpp"

using std::cin;
using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;


string evstr(float num, int width=10) {
    string ev = std::to_string(num);
    if (ev.size() > 6) ev = ev.substr(0, 6);

    int padding = width - 2 - ev.size();
    for (auto i = 0; i < padding; i++) ev += " ";
    ev = "  " + ev;

    return ev;
}


void print_eval(Board board) {
    int movect = board.move_stack().size();
    float mat = material(board);
    float mat_weight = material_weight(movect);
    float mat_final = mat * mat_weight;

    cout << "    Category |   Eval   |  Weight  |  Final\n";
    cout << " ------------+----------+----------+----------\n";
    cout << "    Material |" << evstr(mat) << "|" << evstr(mat_weight) << "|" << evstr(mat_final) << "\n";
    cout << std::flush;
}


void setup_position(Board& board, string cmd) {
    cmd = strip(replace(cmd, "position"));
    if (startswith(cmd, "startpos")) {
        board.reset();
        cmd = strip(replace(cmd, "startpos"));
        if (startswith(cmd, "moves")) {
            vector<string> uci_moves = split(strip(replace(cmd, "moves")), " ");
            for (auto move: uci_moves) board.push_uci(move);
        }
    } else if (startswith(cmd, "fen")) {
        vector<string> parts = split(strip(replace(cmd, "fen")), " ");
        board.set_fen(join(" ", vector<string>(parts.begin(), parts.begin()+6)));
        if (parts.size() >= 7 && parts[6] == "moves") {
            vector<string> uci_moves = vector<string>(parts.begin()+7, parts.end());
            for (auto move: uci_moves) board.push_uci(move);
        }
    }
}


void loop() {
    Board board;
    board.set_fen("8/8/8/8/8/8/8/8 w KQkq - 0 1");
    string cmd;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") cout << "uciok" << endl;

        else if (cmd == "d") cout << board.as_string() << endl;
        else if (cmd == "eval") print_eval(board);

        else if (cmd == "ucinewgame") board = Board();
        else if (startswith(cmd, "position")) setup_position(board, cmd);
        else if (startswith(cmd, "go"));
        else if (cmd == "stop");
    }
}
