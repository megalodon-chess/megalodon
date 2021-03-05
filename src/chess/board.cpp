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

#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include "board.hpp"
#include "constants.hpp"
#include "funcs.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


Board::Board() {
    _board = {
        {BR, BN, BB, BQ, BK, BB, BN, BR},
        {BP, BP, BP, BP, BP, BP, BP, BP},
        {EM, EM, EM, EM, EM, EM, EM, EM},
        {EM, EM, EM, EM, EM, EM, EM, EM},
        {EM, EM, EM, EM, EM, EM, EM, EM},
        {EM, EM, EM, EM, EM, EM, EM, EM},
        {WP, WP, WP, WP, WP, WP, WP, WP},
        {WR, WN, WB, WQ, WK, WB, WN, WR}
    };
    _turn = true;
    _castling = {true, true, true, true};
    _ep = false;
}

Board::Board(string fen) {
    set_fen(fen);
}

void Board::set_fen(string fen) {
    vector<string> parts = split(fen, " ");
    int i = 0;
    string pos;

    for (auto p: parts[0]) {
        if (p != '/') pos.push_back(p);
    }

    string turn = parts[1];
    string castling = parts[2];
    string ep = parts[2];

    for (auto p: pos) {
        if (std::isdigit(p)) {
            int val = i + std::stoi(string(1, p));
            for (auto j = i; j < val; j++) {
                _board[j / 8][j % 8] = EM;
            }
            i = val;
        }
        else {
            _board[i / 8][i % 8] = symbol_to_piece(string(1, p));
            i++;
        }
    }
}

string Board::fen() {
    string fen;
    int space = 0;

    for (auto row = 0; row < 8; row++) {
        for (auto col = 0; col < 8; col++) {
            int sq = _board[row][col];
            if (sq != EM) {
                if (space) fen += std::to_string(space);
                fen += piece_to_symbol(sq);
                space = 0;
            } else {
                space++;
            }

        }
        if (space) fen += std::to_string(space);
        space = 0;
        fen += "/";
    }
    fen.pop_back();

    fen += " ";
    if (_turn) fen += "w";
    else fen += "b";

    bool contains = false;
    for (auto c: _castling) {
        if (c) {
            contains = true;
            break;
        }
    }
    fen += " ";
    if (contains) {
        for (auto i = 0; i < 4; i++) {
            if (_castling[i]) fen += CASTLING_SYMBOLS[i];
        }
    } else {
        fen += "-";
    }

    fen += " ";
    if (_ep) fen += square_to_string(_ep_square);
    else fen += "-";

    fen += " 0 1";
    return fen;
}

void Board::push(Move move) {
    vector<int> to = move.to_square();
    vector<int> from = move.from_square();

    if (move.promotion()) _board[to[1]][to[0]] = move.promo_piece();
    else _board[to[1]][to[0]] = _board[from[1]][from[0]];
    _board[from[1]][from[0]] = EM;
}

void Board::push_uci(string str) {
    push(Move(str));
}

vector<int> Board::king_pos(bool side) {
    int k = side ? WK : BK;
    for (auto row = 0; row < 8; row++) {
        for (auto col = 0; col < 8; col++) {
            if (_board[row][col] == k) return {row, col};
        }
    }
}

vector<vector<int>> Board::board() {
    return _board;
}

std::ostream& operator << (std::ostream& out, const Board& board) {
    out << " " << BOARD_OUTROW << "\n";
    for (auto i = 0; i < 8; i++) {
        vector<int> row = board._board[i];
        out << BOARD_OUTCOL;
        for (auto piece: row) out << piece_to_symbol(piece) << BOARD_OUTCOL;

        out << 8-i << "\n";
        out << " " << BOARD_OUTROW << "\n";
    }
    out << "   ";
    for (auto i: "abcdefgh") out << i << "   ";
    out << endl;

    return out;
}
