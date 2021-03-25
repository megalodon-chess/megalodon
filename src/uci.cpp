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
#include "hash.hpp"

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


void print_legal_moves(Position pos) {
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

float go(Options& options, Position& pos, vector<string> parts, float prev_eval) {
    int mode = 0, depth, total = total_mat(pos);
    float wtime = 0, btime = 0, winc = 0, binc = 0;
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

    if (mode == 0) {
        depth = 7;
    } else if (mode == 2) {
        double time;
        wtime /= 1000;
        btime /= 1000;
        winc /= 1000;
        binc /= 1000;
        if (pos.turn) time = move_time(options, pos, wtime, winc);
        else time = move_time(options, pos, btime, binc);

        if (20 <= time) depth = 6;
        else if (10 <= time && time < 20) depth = 5;
        else depth = 4;
    }
    if (total < 15) depth++;
    if (total < 5) depth++;

    SearchInfo result;
    if (mode == 1) {

    } else { 
        result = search(options, pos, depth);
        cout << "bestmove " << Bitboard::move_str(result.pv[0]) << endl;
    }

    chat(options, pos.turn, pos.move_stack.size(), result.score, prev_eval);
    return result.score;
}

void perft(Options& options, Position pos, int depth) {
    vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
    double start = get_time();
    int nodes = 1;

    if (moves.size() > 0) {
        int move_num = 1;
        for (const auto& move: moves) {
            Position new_pos = Bitboard::push(pos, move);
            int curr_nodes = Perft::movegen(new_pos, depth-1);
            nodes += curr_nodes;
            cout << "info currmove " << Bitboard::move_str(move) << " currmovenumber " << move_num << " nodes " << curr_nodes << endl;
            move_num++;
        }
    }

    double elapse = get_time() - start + 0.001;  // Add 1 ms to prevent divide by 0
    cout << "info depth " << depth << " nodes " << nodes << " nps " << (int)(nodes/elapse) << " time " << (int)(elapse*1000) << endl;
}

void perft_hash(Options& options, Position pos, int knodes) {
    double time = Perft::hash_perft(pos, knodes);
    cout << "info nodes " << 1000*knodes << " nps " << (int)(knodes*1000/time) << " time " << (int)(time*1000) << endl;
}


int loop() {
    string cmd;
    Options options;
    Position pos = parse_pos("position startpos");
    float prev_eval = 0;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);

        if (cmd == "quit") break;
        else if (cmd == "clear") {
            string str;
            for (char i: {27, 91, 51, 74, 27, 91, 72, 27, 91, 50, 74}) str += string(1, i);
            cout << str << std::flush;
        }
        else if (cmd == "isready") cout << "readyok" << endl;
        else if (cmd == "uci") {
            cout << "option name Hash type spin default 16 min 1 max 65536" << "\n";
            cout << "option name UseHashTable type check default false" << "\n";
            cout << "option name EvalMaterial type spin default 100 min 0 max 1000" << "\n";
            cout << "option name Chat type check default false" << "\n";
            cout << "uciok" << endl;
        }
        else if (startswith(cmd, "setoption")) {
            vector<string> parts = split(cmd, " ");
            string name = parts[2];
            string value = parts[4];

            if (name == "Hash") {
                options.Hash = std::stoi(value);
                options.set_hash();
            }
            else if (name == "UseHashTable") options.UseHashTable = (value == "true");
            else if (name == "EvalMaterial") options.EvalMaterial = std::stoi(value);
            else if (name == "Chat") options.Chat = (value == "true");
            else cout << "Unknown option: " << name << endl;
        }

        else if (cmd == "d") cout << Bitboard::board_str(pos) << endl;
        else if (startswith(cmd, "hash")) {
            vector<string> parts = split(cmd, " ");
            if (parts.size() == 1) {
                cout << hash(pos) << endl;
            } else if (parts[1] == "perft" && parts.size() >= 2) {
                perft_hash(options, pos, std::stoi(parts[2]));
            }
        }
        else if (cmd == "eval") {
            U64 attacked = Bitboard::attacked(pos, !pos.turn);
            cout << eval(options, pos, !Bitboard::legal_moves(pos, attacked).empty(), 0, attacked) << endl;
        }
        else if (cmd == "legalmoves") print_legal_moves(pos);

        else if (cmd == "ucinewgame") {
            pos = parse_pos("position startpos");
            prev_eval = 0;
        }
        else if (startswith(cmd, "position")) pos = parse_pos(cmd);
        else if (startswith(cmd, "go")) {
            vector<string> parts = split(cmd, " ");
            if (parts.size() > 1 && parts[1] == "perft") perft(options, pos, std::stoi(parts[2]));
            else prev_eval = go(options, pos, parts, prev_eval);
        }
        else if (cmd == "stop");
        else if (cmd.size() > 0) cout << "Unknown command: " << cmd << endl;
    }

    delete options.hash_evaled;
    delete options.hash_evals;
    return 0;
}
