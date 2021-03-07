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
#include <algorithm>
#include "board.hpp"
#include "constants.hpp"
#include "funcs.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


Board::Board() {
    reset();
}

Board::Board(string fen) {
    set_fen(fen);
    _move_stack = {};
}

Board::Board(vector<vector<int>> pos, vector<bool> castling, bool ep, vector<int> ep_square, vector<Move> move_stack) {
    _board = pos;
    _castling = castling;
    _ep = ep;
    _ep_square = ep_square;
    _move_stack = move_stack;
}

void Board::reset() {
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
    _move_stack = {};
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

Board Board::copy() {
    Board board;

    board._board.clear();
    for (auto row: _board) {
        vector<int> new_row;
        for (auto piece: row) new_row.push_back(piece);
        board._board.push_back(new_row);
    }

    board._castling = _castling;
    board._turn = _turn;
    board._ep = _ep;
    board._ep_square = _ep_square;
    board._move_stack = _move_stack;

    return board;
}

vector<vector<int>> Board::board() {
    return _board;
}

vector<Move> Board::move_stack() {
    return _move_stack;
}

Move Board::peek() {
    return _move_stack.back();
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

    if (castling == "-") {
        _castling = {false, false, false, false};
    } else {
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

bool Board::turn() {
    return _turn;
}

void Board::push(Move move) {
    vector<int> to = move.to_square();
    vector<int> from = move.from_square();

    _board[to[0]][to[1]] = move.promotion() ? move.promo_piece() : _board[from[0]][from[1]];
    _board[from[0]][from[1]] = EM;
    _turn = !_turn;
    _move_stack.push_back(move);
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

bool Board::in_check(bool side) {
    const vector<int> k_pos = king_pos(side);

    for (auto row = 0; row < 8; row++) {
        for (auto col = 0; col < 8; col++) {
            int piece = _board[row][col];
            if (piece_color(piece) == side) break;
            vector<Move> moves;
            switch (piece) {
                case WP: case BP: moves = pawn_moves({row, col}); break;
                case WN: case BN: moves = knight_moves({row, col}); break;
                case WB: case BB: moves = bishop_moves({row, col}); break;
                case WQ: case BQ: moves = queen_moves({row, col}); break;
                case WK: case BK: moves = king_moves({row, col}); break;

                if (std::find_if(moves.begin(), moves.end(), [&](Move m){return m.to_square()==k_pos;}) != moves.end()) return true;
            }
        }
    }
    return false;
}

vector<Move> Board::get_all_legal_moves() {
    vector<Move> moves;
    for (auto row = 0; row < 8; row++) {
        for (auto col = 0; col < 8; col++) {
            int piece = _board[row][col];
            if (piece_color(piece) != _turn) continue;
            vector<Move> new_moves;
            switch (piece) {
                case WP: case BP: new_moves = _filter_moves(pawn_moves({row, col})); break; 
                case WN: case BN: new_moves = _filter_moves(knight_moves({row, col})); break;
                case WB: case BB: new_moves = _filter_moves(bishop_moves({row, col})); break;
                case WQ: case BQ: new_moves = _filter_moves(queen_moves({row, col})); break;
                case WK: case BK: new_moves = _filter_moves(king_moves({row, col})); break;
            }

            moves.reserve(moves.size() + new_moves.size());  // reverse for performance
            moves.insert(moves.end(), new_moves.begin(), new_moves.end());
        }
    }
    return moves;
}

vector<Move> Board::_filter_moves(vector<Move> moves) {
    vector<Move> final_moves;
    for (auto move: moves) {
        Board tmp = copy();
        tmp.push(move);
        if (!tmp.in_check(!tmp.turn())) final_moves.push_back(move);
    }
    return final_moves;
}

vector<Move> Board::_calc_sliding_moves(vector<int> sq, vector<vector<int>> dirs, const int max_dist = 8) {
    vector<Move> moves;
    const string from = square_to_string(sq);

    for (auto dir: dirs) {
        // Loop till edge of board
        for (auto i = 1; i < max_dist+1; i++) {
            // Move in the direction for i distance
            vector<int> sum = {0, 0};
            for (auto j = 0; j < i; j++) sum = addvecs(sum, dir);
            vector<int> new_sq = addvecs(sq, sum);

            if (!in_board(new_sq)) break;  // break if dir has reached out of board
            int piece = _board[new_sq[0]][new_sq[1]];
            if (piece != EM) {
                if (_turn == piece_color(piece)) break;
                moves.push_back(Move(from + square_to_string(new_sq)));
                break;
            } else {
                moves.push_back(Move(from + square_to_string(new_sq)));
            }
        }
    }
    return moves;
}

vector<Move> Board::_calc_jump_moves(vector<int> sq, vector<vector<int>> jumps) {
    vector<Move> moves;
    const string from = square_to_string(sq);

    // For each endpoint in jumps
    for (auto end: jumps) {
        // Check that position
        vector<int> pos = addvecs(sq, end);
        // If position is inside board
        if (in_board(pos)) {
            int piece = _board[pos[0]][pos[1]];
            // If the piece is empty or of opposite color add move
            if (piece == EM || _turn != piece_color(piece)) moves.push_back(Move(from + square_to_string(pos)));
        }
    }
    return moves;
}

vector<Move> Board::rook_moves(vector<int> sq) {
    return _calc_sliding_moves(sq, DIR_R);
}

vector<Move> Board::bishop_moves(vector<int> sq) {
    return _calc_sliding_moves(sq, DIR_B);
}

vector<Move> Board::queen_moves(vector<int> sq) {
    return _calc_sliding_moves(sq, DIR_Q);
}

vector<Move> Board::knight_moves(vector<int> sq) {
    return _calc_jump_moves(sq, DIR_N);
}

vector<Move> Board::king_moves(vector<int> sq) {
    vector<Move> moves = _calc_jump_moves(sq, DIR_K);

    if (_turn) {
        if (_castling[0] && _board[7][5] == EM && _board[7][6] == EM) moves.push_back(Move("e1g1"));
        if (_castling[1] && _board[7][3] == EM && _board[7][2] == EM && _board[7][1]) moves.push_back(Move("e1c1"));
    } else {
        if (_castling[2] && _board[0][5] == EM && _board[0][6] == EM) moves.push_back(Move("e8g8"));
        if (_castling[3] && _board[0][3] == EM && _board[0][2] == EM && _board[0][1]) moves.push_back(Move("e8c8"));
    }
    return moves;
}

vector<Move> Board::pawn_moves(vector<int> sq) {
    vector<Move> moves;
    const string from = square_to_string(sq);
    vector<vector<int>> sides;
    int speed = sq[0] == (_turn ? 6 : 1) ? 3 : 2;  // If white then check rank 6 else rank 1. Depending on if the pawn is there decide speed.

    if (_turn) {
        for (auto r = sq[0] - 1; r > sq[0] - speed; r--) {
            vector<int> pos{r, sq[1]};
            if (!in_board(pos)) break;  // If out of board break
            if (_board[pos[0]][pos[1]] != EM) break;  // If piece in way break
            moves.push_back(Move(from + square_to_string(pos)));
        }
        sides = {{-1, -1}, {-1, 1}};
    } else {
        for (auto r = sq[0] + 1; r < sq[0] + speed; r++) {
            vector<int> pos{r, sq[1]};
            if (!in_board(pos)) break;  // If out of board break
            if (_board[pos[0]][pos[1]] != EM) break;  // If piece in way break
            moves.push_back(Move(from + square_to_string(pos)));
        }
        sides = {{1, -1}, {1, 1}};
    }

    for (auto side: sides) {
        vector<int> pos = addvecs(sq, side);
        if (in_board(pos)) {
            const int piece = _board[pos[0]][pos[1]];
            if (pos == _ep_square || (piece != EM && _turn != piece_color(piece))) {
                moves.push_back(Move(from + square_to_string(pos)));
            }
        }
    }
    return moves;
}

vector<int> Board::attackers(vector<int> sq, bool side) {
    vector<int> attackers;
    int dest = _board[sq[0]][sq[1]];
    // Need to swap turns because otherwise we won't get proper moves. ex: white piece attacking white piece
    bool swap = (dest != EM && piece_color(dest) == side);
    _turn = swap ? !_turn : _turn;

    for (auto row = 0; row < 8; row++) {
        for (auto col = 0; col < 8; col ++) {
            int piece = _board[row][col];
            if (piece == EM || piece_color(piece) != side) break;
            vector<Move> moves;
            switch (piece) {
                case WP: case BP: moves = pawn_moves({row, col}); break;
                case WN: case BN: moves = knight_moves({row, col}); break;
                case WB: case BB: moves = bishop_moves({row, col}); break;
                case WQ: case BQ: moves = queen_moves({row, col}); break;
                case WK: case BK: moves = king_moves({row, col}); break;
            }
            // Increase attack count if square in moves of curr piece.
            if (std::find_if(moves.begin(), moves.end(), [&](Move m){return m.to_square()==sq;}) != moves.end()) attackers.push_back(piece);
        }
    }

    _turn = swap ? !_turn : _turn;
    return attackers;
}

vector<int> Board::get_row(int ind) {
    return _board[ind];
}

vector<int> Board::get_col(int ind) {
    vector<int> col;
    for (auto row: _board) {
        col.push_back(row[ind]);
    }
    return col
}
