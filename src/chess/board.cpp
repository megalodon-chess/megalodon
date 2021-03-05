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

string Board::as_string() {
    string str;
    str += " " + BOARD_OUTROW + "\n";
    for (auto i = 0; i < 8; i++) {
        str += BOARD_OUTCOL;
        for (auto piece: _board[i]) {
            string symbol = piece_to_symbol(piece);
            str += symbol + BOARD_OUTCOL;
        }
        str += std::to_string(8-i) + "\n";
        str += " " + BOARD_OUTROW + "\n";
    }
    str += "   ";
    for (auto i: "abcdefgh") str += string(1, i) + "   ";

    str += "\n\nFen: " + fen();
    return str;
}

void Board::set_fen(string fen) {
    vector<string> parts = split(fen, " ");
    string position = parts[0];
    string turn = parts[1];
    string castling = parts[2];
    string ep = parts[3];

    _board = {};
    vector<int> row = {};
    for (auto i = 0; i < position.size(); i++) {
        char ch = position[i];
        if (ch == '/') {
            _board.push_back(row);
            row = {};
        } else {
            if (48 <= ch && ch <= 57) {
                for (auto j = 0; j < ch-48; j++) row.push_back(EM);
            } else {
                row.push_back(symbol_to_piece(string(1, ch)));
            }
        }
    }
    _board.push_back(row);

    _turn = (turn == "w");

    for (auto i: castling) {
        int ind;
        switch (i) {
            case 'K': ind = 0; break;
            case 'Q': ind = 1; break;
            case 'k': ind = 2; break;
            case 'q': ind = 3; break;
        }
        _castling[ind] = true;
    }

    _ep = (ep != "-");
    if (_ep) _ep_square = string_to_square(ep);
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
    fen += _turn ? "w" : "b";

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
    fen += _ep ? square_to_string(_ep_square) : "-";

    fen += " 0 1";
    return fen;
}

void Board::push(Move move) {
    vector<int> to = move.to_square();
    vector<int> from = move.from_square();

    _board[to[1]][to[0]] = move.promotion() ? move.promo_piece() : _board[from[1]][from[0]];
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


// Legal moves

vector<Move> Board::rook_moves(vector<int> sq) {
    vector<Move> moves;
    string curr = square_to_string(sq);
    // Up
    for (auto r = sq[0]; r > 0; r--) {
        vector<int> cell = {r, sq[1]};
        int piece = _board[cell[0]][cell[1]];
        if (piece != EM) {
            bool cell_col = piece_color(piece);
            if (_turn == cell_col) break;
            moves.push_back(Move(curr + square_to_string(cell)));
            if (_turn != cell_col) break;
        } else {
            moves.push_back(Move(curr + square_to_string(cell)));
        }
    }
    // Down
    for (auto r = sq[0]; r > 8; r++) {
        vector<int> cell = {r, sq[1]};
        int piece = _board[cell[0]][cell[1]];
        if (piece != EM) {
            bool cell_col = piece_color(piece);
            if (_turn == cell_col) break;
            moves.push_back(Move(curr + square_to_string(cell)));
            if (_turn != cell_col) break;
        } else {
            moves.push_back(Move(curr + square_to_string(cell)));
        }
    }
    // Left
    for (auto c = sq[1]; c > 0; c--) {
        vector<int> cell = {sq[0], c};
        int piece = _board[cell[0]][cell[1]];
        if (piece != EM) {
            bool cell_col = piece_color(piece);
            if (_turn == cell_col) break;
            moves.push_back(Move(curr + square_to_string(cell)));
            if (_turn != cell_col) break;
        } else {
            moves.push_back(Move(curr + square_to_string(cell)));
        }
    }
    // Right
    for (auto c = sq[0]; c > 8; c++) {
        vector<int> cell = {sq[0], c};
        int piece = _board[cell[0]][cell[1]];
        if (piece != EM) {
            bool cell_col = piece_color(piece);
            if (_turn == cell_col) break;
            moves.push_back(Move(curr + square_to_string(cell)));
            if (_turn != cell_col) break;
        } else {
            moves.push_back(Move(curr + square_to_string(cell)));
        }
    }
    return moves;
}
