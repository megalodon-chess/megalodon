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
#include "utils.hpp"
#include "bitboard.hpp"
#include "search.hpp"
#include "options.hpp"
#include "eval.hpp"
#include "perft.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;

vector<string> GREETINGS = {"Hello!", "Lets play!", "Are you ready for a game?"};


Position parse_pos(string str) {
    vector<string> parts = split(str, " ");
    if (parts[1] == "startpos") {
        Position pos = Bitboard::startpos();
        if (parts.size() > 3 && parts[2] == "moves") {
            for (auto i = 3; i < parts.size(); i++) {
                pos = Bitboard::push(pos, parts[i]);
            }
        }
        return pos;
    } else if (parts[1] == "fen") {
        string fen;
        for (auto i = 2; i < 8; i++) {
            fen += parts[i];
            fen += " ";
        }
        Position pos = Bitboard::parse_fen(fen);

        if (parts.size() > 9 && parts[8] == "moves") {
            for (auto i = 9; i < parts.size(); i++) {
                pos = Bitboard::push(pos, parts[i]);
            }
        }
        return pos;
    }
    return Position();
}


void legal_moves(Position pos) {
    vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, pos.turn));
    cout << "Legal moves: " << endl;
    if (moves.empty()) {
        cout << "None" << endl;
    } else {
        for (auto m: moves) cout << Bitboard::move_str(m) << endl;
    }
}


void chat(Options& options, int movect) {
    if (!options.Chat) return;

    if (movect == 0) {
        cout << "info string " << GREETINGS[rand()%GREETINGS.size()] << endl;
    }
}


void print_eval(Options& options, Position pos) {
    int movect = pos.move_stack.size();
    U64 w_attacks = Bitboard::attacked(pos, true);
    U64 b_attacks = Bitboard::attacked(pos, false);

    float mat = material(pos);
    float mat_weight = material_weight(options, movect);
    float cent = center(w_attacks, b_attacks);
    float cent_weight = center_weight(options, movect);

    vector<vector<string>> evals = {
        {"Material", to_string(mat), to_string(mat_weight), to_string(mat*mat_weight)},
        {"Center Control", to_string(cent), to_string(cent_weight), to_string(cent*cent_weight)}
    };

    for (auto ev: evals) {
        string category = ev[0];
        string value = ev[1];
        string weight = ev[2];
        string total = ev[3];
        if (value.size() > 8) value = value.substr(0, 8);
        if (weight.size() > 8) weight = weight.substr(0, 8);
        if (total.size() > 8) total = total.substr(0, 8);

        cout << category << ": ";
        for (auto i = 0; i < 18-category.size(); i++) cout << " ";
        cout << value << " x ";
        for (auto i = 0; i < 8-value.size(); i++) cout << " ";
        cout << weight << " = ";
        for (auto i = 0; i < 8-weight.size(); i++) cout << " ";
        cout << total;
        for (auto i = 0; i < 8-total.size(); i++) cout << " ";
        cout << endl;
    }
}


int loop() {
    string cmd;
    Options options;
    Position pos = parse_pos("position startpos");

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "option name EvalMaterial type spin default 100 min 0 max 1000" << "\n";
            cout << "option name EvalCenter type spin default 100 min 0 max 1000" << "\n";
            cout << "option name Chat type check default false" << "\n";
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];

            if (name == "EvalMaterial") options.EvalMaterial = std::stoi(value);
            else if (name == "EvalCenter") options.EvalCenter = std::stoi(value);
            else if (name == "Chat") options.Chat = (value == "true");
        }
        else if (startswith(cmd, "getoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];

            if (name == "EvalMaterial") cout << options.EvalMaterial << endl;
            else if (name == "EvalCenter") cout << options.EvalCenter << endl;
            else if (name == "Chat") cout << options.Chat << endl;
        }

        else if (cmd == "d") cout << Bitboard::board_str(pos) << endl;
        else if (cmd == "eval") print_eval(options, pos);
        else if (startswith(cmd, "perft")) {
            vector<string> parts = split(cmd, " ");
            if (parts[1] == "movegen") Perft::movegen(pos, std::stoi(parts[2]));
        }
        else if (cmd == "legalmoves") legal_moves(pos);

        else if (cmd == "ucinewgame");
        else if (startswith(cmd, "position")) pos = parse_pos(cmd);
        else if (startswith(cmd, "go")) {
            SearchInfo result = search(options, pos, 4);
            cout << result.as_string() << endl;
            chat(options, pos.move_stack.size());
        }
        else if (cmd == "stop");
    }

    return 0;
}
