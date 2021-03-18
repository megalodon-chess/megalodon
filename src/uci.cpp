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
vector<string> WINNING = {"Looks like I'm playing well!", "Wow!", "This is great!"};
vector<string> LOSING = {"Oh no!", "I blundered.", "Nice play!", "Great job!", "*sigh*. You're good."};
vector<string> GAME_END = {"Good game!", "I look forward to playing again.", "Want to play another one?", "Rematch?"};


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
    vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
    cout << moves.size() << endl;
    for (auto m: moves) cout << Bitboard::move_str(m) << "\n";
}


void chat(Options& options, bool turn, int movect, float score, float prev_score) {
    if (!options.Chat) return;

    if (movect == 0) cout << "info string " << rand_choice(GREETINGS) << endl;
    if (turn && (score > (prev_score+1.5))) cout << "info string " << rand_choice(WINNING) << endl;
    if (!turn && (score < (prev_score-1.5))) cout << "info string " << rand_choice(WINNING) << endl;
    if (turn && (score < (prev_score-1.5))) cout << "info string " << rand_choice(LOSING) << endl;
    if (!turn && (score > (prev_score+1.5))) cout << "info string " << rand_choice(LOSING) << endl;
}


int loop() {
    string cmd;
    Options options;
    Position pos = parse_pos("position startpos");
    float prev_eval = 0;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "option name EvalMaterial type spin default 100 min 0 max 1000" << "\n";
            cout << "option name Chat type check default false" << "\n";
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];

            if (name == "EvalMaterial") options.EvalMaterial = std::stoi(value);
            else if (name == "Chat") options.Chat = (value == "true");
            else cout << "Unknown option: " << name << endl;
        }

        else if (cmd == "d") cout << Bitboard::board_str(pos) << endl;
        else if (cmd == "eval") cout << eval(options, pos, true, 0) << endl;
        else if (startswith(cmd, "perft")) {
            vector<string> parts = split(cmd, " ");
            if (parts[1] == "movegen") {
                double start = get_time();
                int count = Perft::movegen(pos, std::stoi(parts[2]));
                cout << "info nodes " << count << " time " << get_time()-start << endl;
            }
        }
        else if (cmd == "legalmoves") legal_moves(pos);

        else if (cmd == "ucinewgame") {
            pos = parse_pos("position startpos");
            prev_eval = 0;
        }
        else if (startswith(cmd, "position")) pos = parse_pos(cmd);
        else if (startswith(cmd, "go")) {
            vector<string> parts = split(cmd, " ");
            int mode = 0, depth, total = total_mat(pos);
            float wtime, btime, winc, binc;
            for (auto i = 0; i < parts.size(); i++) {
                if (parts[i] == "depth") {
                    mode = 1;
                    depth = std::stoi(parts[i+1]);
                    break;
                } else if (parts[i] == "wtime") {
                    mode = 2;
                    wtime = std::stoi(parts[i+1]);
                } else if (parts[i] == "btime") {
                    mode = 2;
                    btime = std::stoi(parts[i+1]);
                } else if (parts[i] == "winc") {
                    mode = 2;
                    winc = std::stoi(parts[i+1]);
                } else if (parts[i] == "binc") {
                    mode = 2;
                    binc = std::stoi(parts[i+1]);
                }
            }
            wtime /= 1000;
            btime /= 1000;
            winc /= 1000;
            binc /= 1000;

            if (mode == 0) {
                depth = 5;
            } else if (mode == 2) {
                double time;
                if (pos.turn) time = move_time(options, pos, wtime, winc);
                else time = move_time(options, pos, btime, binc);

                if (15 <= time) depth = 5;
                else if (1 <= time && time < 15) depth = 4;
                else depth = 3;
            }
            if (total < 20) depth++;
            if (total < 10) depth++;

            double start = get_time();
            SearchInfo result = search(options, pos, MIN, MAX, true, depth, 0);
            double elapse = get_time() - start;

            float score = result.score;
            result.time = 1000 * (elapse);
            result.nps = result.nodes / (elapse);
            if (!pos.turn) result.score *= -1;
            cout << result.as_string() << endl;
            cout << "bestmove " << Bitboard::move_str(result.move) << endl;

            chat(options, pos.turn, pos.move_stack.size(), score, prev_eval);
            prev_eval = result.score;
        }
        else if (cmd == "stop");
        else if (cmd.size() > 0) cout << "Unknown command: " << cmd << endl;
    }

    return 0;
}
