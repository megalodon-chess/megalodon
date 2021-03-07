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
#include "options.hpp"
#include "search.hpp"

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


void print_eval(Board board, Options& options) {
    int movect = board.move_stack().size();

    cout << "\n";
    cout << "        Category |   Eval   |  Weight  |  Final\n";
    cout << " ----------------+----------+----------+----------\n";

    float mat = material(board);
    float mat_weight = material_weight(movect);
    cout << "        Material |" << evstr(mat) << "|" << evstr(mat_weight) << "|" << evstr(mat*mat_weight) << "\n";

    if (options.UsePieceMaps) {
        float pm = piece_map(board, options);
        float pm_weight = piece_map_weight(movect);
        cout << "       Piece Map |" << evstr(pm) << "|" << evstr(pm_weight) << "|" << evstr(pm*pm_weight) << "\n";
    } else {
        cout << "       Piece Map | Disabled | Disabled | Disabled";
    }

    float cent = center(board);
    float cent_weight = center_weight(movect);
    cout << "  Centralization |" << evstr(cent) << "|" << evstr(cent_weight) << "|" << evstr(cent*cent_weight) << "\n";

    cout << endl;
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
    Options options;
    Board board;
    Tree tree;
    string cmd;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "option name UsePieceMaps type check default true\n";
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];

            if (name == "UsePieceMaps") options.UsePieceMaps = (value == "true");
        }

        else if (cmd == "d") cout << board.as_string() << endl;
        else if (cmd == "eval") print_eval(board, options);

        else if (cmd == "ucinewgame") board = Board();
        else if (startswith(cmd, "position")) setup_position(board, cmd);
        else if (startswith(cmd, "go")) {
            tree.go_depth(options, board, 3);
        }
        else if (cmd == "stop") {
            if (tree.active()) tree.stop(options);
        }
    }
}
