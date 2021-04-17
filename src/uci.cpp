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
#include <thread>
#include "utils.hpp"
#include "bitboard.hpp"
#include "search.hpp"
#include "options.hpp"
#include "eval.hpp"
#include "perft.hpp"
#include "hash.hpp"
#include "endgame.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::to_string;

const vector<string> GREETINGS = {"Hello!", "Lets play!", "Are you ready for a game?"};
const vector<string> WINNING = {"Looks like I'm playing well!", "Wow!", "This is great!"};
const vector<string> LOSING = {"Oh no!", "I blundered.", "Nice play!", "Great job!", "*sigh*. You're good."};
const vector<string> GAME_END = {"Good game!", "I look forward to playing again.", "Want to play another one?", "Rematch?"};


Position parse_pos(const vector<string>& parts) {
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


void print_legal_moves(const Position& pos) {
    const vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
    cout << moves.size() << endl;
    for (const auto& m: moves) cout << Bitboard::move_str(m) << "\n";
}

void chat(const Options& options, const bool& turn, const int& movect, const float& score, const float& prev_score) {
    return;  //! CHAT IS DISABLED
    if (movect == 0) cout << "info string " << rand_choice(GREETINGS) << endl;
    else if (turn  && (score > (prev_score+1.5))) cout << "info string " << rand_choice(WINNING) << endl;
    else if (!turn && (score < (prev_score-1.5))) cout << "info string " << rand_choice(WINNING) << endl;
    else if (turn  && (score < (prev_score-1.5))) cout << "info string " << rand_choice(LOSING)  << endl;
    else if (!turn && (score > (prev_score+1.5))) cout << "info string " << rand_choice(LOSING)  << endl;
}

float go(const Options& options, const Position& pos, const vector<string>& parts, const float& prev_eval, bool& searching) {
    int mode = 0;
    int depth = 99;
    double movetime;
    const int total = Eval::total_mat(pos);
    float wtime = 0, btime = 0, winc = 0, binc = 0;
    bool infinite = false;
    for (auto i = 0; i < parts.size()-1; i++) {
        if (parts[i] == "depth") {
            mode = 1;
            depth = std::stoi(parts[i+1]);
        } else if (parts[i] == "wtime") {
            mode = 2;
            wtime = std::stof(parts[i+1]) / 1000;
        } else if (parts[i] == "btime") {
            mode = 2;
            btime = std::stof(parts[i+1]) / 1000;
        } else if (parts[i] == "winc") {
            mode = 2;
            winc = std::stof(parts[i+1]) / 1000;
        } else if (parts[i] == "binc") {
            mode = 2;
            binc = std::stof(parts[i+1]) / 1000;
        } else if (parts[i] == "movetime") {
            mode = 3;
            movetime = std::stof(parts[i+1]) / 1000;
        } else if (parts[i] == "infinite") {
            infinite = true;
        }
    }

    if (mode == 0) {
        depth = 99;
        movetime = 10000000;  // About 100 days
    } else if (mode == 1) {
        movetime = 10000000;
    } else if (mode == 2) {
        depth = 99;
        if (pos.turn) movetime = Search::move_time(options, pos, wtime, winc);
        else          movetime = Search::move_time(options, pos, btime, binc);
    }
    if (mode == 2) movetime /= 1.5;

    searching = true;
    const SearchInfo result = Search::search(options, pos, depth, movetime, infinite, searching, (mode==2));
    cout << "bestmove " << Bitboard::move_str(result.pv[0]) << endl;

    chat(options, pos.turn, pos.move_cnt, result.score, prev_eval);
    return result.score;
}

void perft(const Options& options, const Position& pos, const int& depth) {
    const vector<Move> moves = Bitboard::legal_moves(pos, Bitboard::attacked(pos, !pos.turn));
    const double start = get_time();
    long long nodes = 0;

    if (moves.size() > 0) {
        int move_num = 1;
        for (const auto& move: moves) {
            const Position new_pos = Bitboard::push(pos, move);
            const long long curr_nodes = Perft::movegen(new_pos, depth-1);
            nodes += curr_nodes;
            cout << "info currmove " << Bitboard::move_str(move) << " currmovenumber " << move_num << " nodes " << curr_nodes << endl;
            move_num++;
        }
    }

    const double elapse = get_time() - start + 0.001;  // Add 1 ms to prevent divide by 0
    cout << "info depth " << depth << " nodes " << nodes << " nps " << (long long)(nodes/elapse) << " time " << (long long)(elapse*1000) << endl;
}

void perft_hash(const Options& options, const Position& pos, const int& knodes) {
    const double time = Perft::hash_perft(pos, knodes);
    cout << "info nodes " << 1000*knodes << " nps " << (int)(knodes*1000/time) << " time " << (int)(time*1000) << endl;
}

void perft_eval(const Options& options, const Position& pos, const int& knodes) {
    const double time = Perft::eval_perft(options, pos, knodes);
    cout << "info nodes " << 1000*knodes << " nps " << (int)(knodes*1000/time) << " time " << (int)(time*1000) << endl;
}

void perft_push(const Position& pos, const int& knodes) {
    const double time = Perft::push_perft(pos, knodes);
    cout << "info nodes " << 1000*knodes << " nps " << (int)(knodes*1000/time) << " time " << (int)(time*1000) << endl;
}

void bench() {
    const Options options;
    constexpr char num_pos = 20;
    constexpr char depth = 4;
    const string fens[num_pos] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        "r2q1rk1/ppp2pp1/2np1n1p/2b1p3/2B1P1b1/2NPBN2/PPPQ1PPP/R3R1K1 b Qq - 0 1",
        "6k1/5ppp/8/8/8/8/5PPP/6K1 w - - 0 1",
        "8/8/NnNnNnNn/8/8/8/8/2k2K2 b - - 0 1",
        "7k/p6P/p6P/p6P/p6P/p6P/p6P/K7 w - - 0 1",
        "8/8/8/8/8/2k5/8/1QK5 w - - 0 1",
        "4k3/p7/Pp6/1Pp1p3/2PpPp1p/3P1PpP/6P1/3K4 w - - 0 1",
        "8/1k6/3K4/2P5/8/8/8/8 w - - 0 1",
        "r2q1rk1/pbp1bpp1/1pn1pn1p/3p4/2PP3B/1PN1PN2/P3BPPP/R2Q1RK1 b Qq - 0 1",
        "2k4r/1pp5/p7/2p1N3/3n1P2/7P/6P1/R5K1 b - - 0 1",
        "7k/p7/8/8/8/8/P7/7K b - - 0 1",
        "8/8/8/3b4/8/3Bk3/1BK2b2/8 b - - 0 1",
        "8/3qPK2/8/8/8/4k3/8/8 w - - 0 1",
        "8/7P/5K2/8/8/4k3/p7/8 w - - 0 1",
        "r5k1/2p2ppp/pp2n3/8/P7/1B1P3P/1PP2PP1/3R1RK1 b - - 0 1",
        "4k3/pppppppp/8/8/8/8/PPPPPPPP/4K3 b - - 0 1",
        "r1r1r1r1/k7/8/8/8/8/1K6/1R1R1R1R w - - 0 1",
        "K7/8/8/3P4/4p3/8/8/7k b - - 0 1",
        "8/kpP3p1/p2n2P1/3Q3P/5q2/P7/KP6/8 w - - 0 1",
        "8/8/8/8/8/k7/8/K7 w - - 0 1",
    };

    U64 nodes = 0;
    const double start = get_time();
    bool searching = true;

    for (char i = 0; i < num_pos; i++) {
        cout << "Position " << i+1 << " of " << +num_pos << endl;
        cout << "Fen: " << fens[i] << endl;

        const Position pos = Bitboard::parse_fen(fens[i]);
        const SearchInfo result = Search::search(options, pos, depth, 10000, false, searching, false);
        nodes += result.nodes;
    }

    const double elapse = get_time() - start;
    const int nps = nodes / elapse;

    cout << "\nBenchmark results:" << endl;
    cout << "Nodes: " << nodes << endl;
    cout << "NPS: " << nps << endl;
    cout << "Time: " << elapse << " seconds" << endl;
}

void parse_command(Options& options, Position& pos, float& prev_eval, bool& searching, const vector<string>& parts, const bool& arg=false) {
    if (parts[0] == "quit") return;
    else if (parts[0] == "clear") {
        string str;
        for (const char& i: {27, 91, 51, 74, 27, 91, 72, 27, 91, 50, 74}) str += string(1, i);
        cout << str << std::flush;
    }
    else if (parts[0] == "isready") cout << "readyok" << endl;
    else if (parts[0] == "uci") {
        cout << "id name Megalodon" << "\n";
        cout << "id author Megalodon Developers" << "\n";

        cout << "option name Hash type spin default 256 min 1 max 65536" << "\n";

        cout << "option name EvalMaterial type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalPawnStruct type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalSpace type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalKnights type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalRooks type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalQueens type spin default 100 min 0 max 1000" << "\n";
        cout << "option name EvalKings type spin default 100 min 0 max 1000" << "\n";

        cout << "uciok" << endl;
    }
    else if (parts[0] == "setoption") {
        const string name = parts[2];
        const string value = parts[4];

        if (name == "Hash") {
            options.Hash = std::stoi(value);
            options.set_hash();
        }

        else if (name == "EvalMaterial")   options.EvalMaterial   = std::stof(value)/100;
        else if (name == "EvalPawnStruct") options.EvalPawnStruct = std::stof(value)/100;
        else if (name == "EvalSpace")      options.EvalSpace      = std::stof(value)/100;
        else if (name == "EvalKnights")    options.EvalKnights    = std::stof(value)/100;
        else if (name == "EvalRooks")      options.EvalRooks      = std::stof(value)/100;
        else if (name == "EvalQueens")     options.EvalQueens     = std::stof(value)/100;
        else if (name == "EvalKings")      options.EvalKings      = std::stof(value)/100;

        else std::cerr << "Unknown option: " << name << endl;
    }

    else if (parts[0] == "d") cout << Bitboard::board_str(pos) << endl;
    else if (parts[0] == "hash") {
        if (parts.size() == 1) {
            cout << Hash::hash(pos) << endl;
        } else if (parts[1] == "perft" && parts.size() >= 2) {
            perft_hash(options, pos, std::stoi(parts[2]));
        }
    }
    else if (parts[0] == "eval") {
        if (parts.size() == 1) {
            const U64 attacked = Bitboard::attacked(pos, !pos.turn);
            Eval::eval(options, pos, Bitboard::legal_moves(pos, attacked), 0, attacked, true);
        } else if (parts[1] == "perft" && parts.size() >= 2) {
            perft_eval(options, pos, std::stoi(parts[2]));
        }
    }
    else if (parts[0] == "bench") bench();
    else if (parts[0] == "legalmoves") print_legal_moves(pos);
    else if (parts[0] == "pushperft") perft_push(pos, std::stoi(parts[1]));
    else if (parts[0] == "eg") cout << Endgame::eg_type(pos) << endl;

    else if (parts[0] == "ucinewgame") {
        pos = parse_pos({"position", "startpos"});
        prev_eval = 0;
    }
    else if (parts[0] == "position") pos = parse_pos(parts);
    else if (parts[0] == "go") {
        if (parts.size() > 1 && parts[1] == "perft") perft(options, pos, std::stoi(parts[2]));
        else {
            options.clear_hash();
            searching = true;
            if (arg) go(options, pos, parts, prev_eval, searching);
            else std::thread(go, options, pos, parts, prev_eval, std::ref(searching)).detach();
        }
    }
    else if (parts[0] == "stop") searching = false;
    else if (parts.size() > 0) std::cerr << "Unknown command: " << join(" ", parts) << endl;
}


int loop() {
    string cmd;
    Options options;
    Position pos = parse_pos({"position", "startpos"});
    float prev_eval = 0;
    bool searching = false;

    while (getline(cin, cmd)) {
        cmd = strip(cmd);
        const vector<string> parts = split(cmd, " ");
        parse_command(options, pos, prev_eval, searching, parts);
    }

    searching = false;
    delete[] options.hash_table;
    return 0;
}
