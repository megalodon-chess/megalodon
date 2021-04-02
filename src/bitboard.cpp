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
#include <cmath>
#include <algorithm>
#include <tuple>
#include "bitboard.hpp"
#include "utils.hpp"
#include "hash.hpp"

using std::abs;
using std::cin;
using std::get;
using std::cout;
using std::endl;
using std::tuple;
using std::vector;
using std::string;


Move::Move() {
}

Move::Move(char _from, char _to, bool _is_promo, char _promo) {
    from = _from;
    to = _to;
    is_promo = _is_promo;
    promo = _promo;
}


Position::Position() {
    wp = Bitboard::EMPTY;
    wn = Bitboard::EMPTY;
    wb = Bitboard::EMPTY;
    wr = Bitboard::EMPTY;
    wq = Bitboard::EMPTY;
    wk = Bitboard::EMPTY;
    bp = Bitboard::EMPTY;
    bn = Bitboard::EMPTY;
    bb = Bitboard::EMPTY;
    br = Bitboard::EMPTY;
    bq = Bitboard::EMPTY;
    bk = Bitboard::EMPTY;

    turn = true;
    castling = 0;
    ep = false;
    ep_square = 0;
    move_cnt = 0;
}

Position::Position(U64 _wp, U64 _wn, U64 _wb, U64 _wr, U64 _wq, U64 _wk, U64 _bp, U64 _bn, U64 _bb, U64 _br, U64 _bq, U64 _bk,
        bool _turn, char _castling, bool _ep, char _ep_square) {
    wp = _wp;
    wn = _wn;
    wb = _wb;
    wr = _wr;
    wq = _wq;
    wk = _wk;
    bp = _bp;
    bn = _bn;
    bb = _bb;
    br = _br;
    bq = _bq;
    bk = _bk;

    turn = _turn;
    castling = _castling;
    ep = _ep;
    ep_square = _ep_square;
    move_cnt = 0;
}


Location::Location() {
    x = 0;
    y = 0;
    loc = 0;
}

Location::Location(char _x, char _y) {
    x = _x;
    y = _y;
    loc = _y<<3+x;
}

Location::Location(char _loc) {
    x = _loc & 7;
    y = _loc >> 3;
    loc = _loc;
}


namespace Bitboard {
    bool bit(const U64& board, const char& pos) {
        return ((1ULL << pos) & board) != 0;
    }

    bool bit(const char& board, const char& pos) {
        return ((1ULL << pos) & board) != 0;
    }

    char popcnt(const U64& num) {
        char count = 0;
        count += popcnt_tbl[ num      & 255ULL];
        count += popcnt_tbl[(num>>8)  & 255ULL];
        count += popcnt_tbl[(num>>16) & 255ULL];
        count += popcnt_tbl[(num>>24) & 255ULL];
        count += popcnt_tbl[(num>>32) & 255ULL];
        count += popcnt_tbl[(num>>40) & 255ULL];
        count += popcnt_tbl[(num>>48) & 255ULL];
        count += popcnt_tbl[(num>>56) & 255ULL];
        return count;
    }

    void set_bit(U64& board, const char& pos) {
        board |= (1ULL << pos);
    }

    void unset_bit(U64& board, const char& pos) {
        board &= ~(1ULL << pos);
    }

    void set_bit(char& board, const char& pos) {
        board |= (1ULL << pos);
    }

    void unset_bit(char& board, const char& pos) {
        board &= ~(1ULL << pos);
    }

    bool in_board(const char& x, const char& y) {
        return (0 <= x && x < 8 && 0 <= y && y < 8);
    }

    Location first_bit(const U64& board) {
        const char pos = log2(board & -board);
        return Location((pos&7), (pos>>3));
    }


    string piece_at(const Position& pos, const char& loc) {
        if (bit(pos.wp, loc)) return "P";
        else if (bit(pos.wn, loc)) return "N";
        else if (bit(pos.wb, loc)) return "B";
        else if (bit(pos.wr, loc)) return "R";
        else if (bit(pos.wq, loc)) return "Q";
        else if (bit(pos.wk, loc)) return "K";
        else if (bit(pos.bp, loc)) return "p";
        else if (bit(pos.bn, loc)) return "n";
        else if (bit(pos.bb, loc)) return "b";
        else if (bit(pos.br, loc)) return "r";
        else if (bit(pos.bq, loc)) return "q";
        else if (bit(pos.bk, loc)) return "k";
        else return " ";
    }

    U64 color(const Position& pos, const bool& color) {
        if (color) return pos.wp | pos.wn | pos.wb | pos.wr | pos.wq | pos.wk;
        else return pos.bp | pos.bn | pos.bb | pos.br | pos.bq | pos.bk;
    }

    string board_str(const U64& board, const string on, const string off) {
        vector<string> rows;
        string repr = "";

        for (auto y = 0; y < 8; y++) {
            string row = "";
            for (auto x = 0; x < 8; x++) {
                row += bit(board, 8*y+x) ? on : off;
                row += " ";
            }
            rows.push_back(row);
        }

        for (auto i = 7; i >= 0; i--) {
            repr += rows[i];
            repr += "\n";
        }

        return repr;
    }

    string board_str(const Position& pos) {
        const string row = " +---+---+---+---+---+---+---+---+";
        const string col = " | ";
        string str = "";

        str += row + "\n";
        for (auto y = 7; y >= 0; y--) {
            for (auto x = 0; x < 8; x++) {
                const char loc = 8*y + x;
                string symbol = piece_at(pos, loc);
                str += col;
                str += symbol;
            }
            str += col + "\n" + row + "\n";
        }
        str += "\nFen: " + fen(pos) + "\n";
        str += "Hash: " + std::to_string(Hash::hash(pos)) + "\n";

        return str;
    }

    string square_str(const char& sq) {
        char x = (sq&7), y = (sq>>3);
        return string(1, x+97) + std::to_string(y+1);
    }

    string move_str(const Move& move) {
        string str;
        str += square_str(move.from);
        str += square_str(move.to);
        if (move.is_promo) {
            switch (move.promo) {
                case 0: str += "n"; break;
                case 1: str += "b"; break;
                case 2: str += "r"; break;
                case 3: str += "q"; break;
            }
        }
        return str;
    }

    string fen(const Position& pos) {
        string str = "";
        char blank_count = 0;

        for (auto y = 7; y >= 0; y--) {
            for (auto x = 0; x < 8; x++) {
                string piece = piece_at(pos, 8*y+x);
                if (piece == " ") {
                    blank_count++;
                } else {
                    if (blank_count > 0) {
                        str += std::to_string(blank_count);
                        blank_count = 0;
                    }
                    str += piece;
                }
            }
            if (blank_count > 0) str += std::to_string(blank_count);
            if (y != 0) str += "/";
            blank_count = 0;
        }

        str += (pos.turn ? " w " : " b ");

        if (pos.castling == 0) str += "-";
        else {
            if (bit(pos.castling, 0)) str += "K";
            if (bit(pos.castling, 1)) str += "Q";
            if (bit(pos.castling, 2)) str += "k";
            if (bit(pos.castling, 3)) str += "q";
        }
        str += " ";

        if (pos.ep) str += square_str(pos.ep_square);
        else str += "-";
        str += " ";

        str += "0 ";
        str += std::to_string(pos.move_cnt/2 + 1);

        return str;
    }

    Position parse_fen(const string& fen) {
        vector<string> parts = split(fen, " ");
        Position pos;

        char x = 0, y = 7;
        for (const auto& ch: parts[0]) {
            if (ch == '/') {
                y--;
                x = 0;
            } else {
                const char loc = 8*y + x;
                if (48 <= ch && ch <= 57) {
                    x += (ch-48);
                } else {
                    switch (ch) {
                        case 'P': set_bit(pos.wp, loc); break;
                        case 'N': set_bit(pos.wn, loc); break;
                        case 'B': set_bit(pos.wb, loc); break;
                        case 'R': set_bit(pos.wr, loc); break;
                        case 'Q': set_bit(pos.wq, loc); break;
                        case 'K': set_bit(pos.wk, loc); break;
                        case 'p': set_bit(pos.bp, loc); break;
                        case 'n': set_bit(pos.bn, loc); break;
                        case 'b': set_bit(pos.bb, loc); break;
                        case 'r': set_bit(pos.br, loc); break;
                        case 'q': set_bit(pos.bq, loc); break;
                        case 'k': set_bit(pos.bk, loc); break;
                    }
                    x++;
                }
            }
        }

        pos.turn = (parts[1] == "w");

        char castling = 0;
        for (const auto& ch: parts[2]) {
            switch (ch) {
                case 'K': castling += 1; break;
                case 'Q': castling += 2; break;
                case 'k': castling += 4; break;
                case 'q': castling += 8; break;
            }
        }
        pos.castling = castling;

        if (parts[3] == "-") pos.ep = false;
        else {
            pos.ep = true;
            pos.ep_square = ((std::stoi(string(1, parts[3][1]))-1)<<3) + (parts[3][0] - 97);
        }

        return pos;
    }

    Move parse_uci(const string& uci) {
        Move move;
        move.from = uci[0]-97 + 8*(uci[1]-49);
        move.to = uci[2]-97 + 8*(uci[3]-49);
        move.is_promo = (uci.size() >= 5);
        if (move.is_promo) {
            switch (uci[4]) {
                case 'N': case 'n': move.promo = 0; break;
                case 'B': case 'b': move.promo = 1; break;
                case 'R': case 'r': move.promo = 2; break;
                case 'Q': case 'q': move.promo = 3; break;
            }
        }
        return move;
    }


    U64 attacked(const U64& pawns, const U64& knights, const U64& bishops, const U64& rooks,
            const U64& queens, const U64& kings, const U64& opponent, const bool& side) {
        /*
        Calculates a bitboard of attacked squares.
        pawns, knights, ...: Bitboards from the attacking side.
        opponent: Bitboard of all pieces from other side.
        side: The side that is attacking.
        return: Bitboard of all attacked squares.
        */
        const U64 pieces = pawns | knights | bishops | rooks | queens | kings | opponent;
        const char pawn_dir = side ? 1 : -1;
        U64 board = EMPTY;

        for (char i = 0; i < 64; i++) {
            const char x = (i&7), y = (i>>3);
            if (bit(pawns, i)) {
                const char ny = y + pawn_dir;  // Current (x, y) with y as after capture.
                if (0 <= ny && ny < 8) {
                    if (0 <= x-1 && x-1 < 8) set_bit(board, (ny<<3) + x-1);
                    if (0 <= x+1 && x+1 < 8) set_bit(board, (ny<<3) + x+1);
                }
            }
            if (bit(knights, i)) {
                for (const auto& dir: DIR_N) {                 // Iterate through all knight moves.
                    const char nx = x+dir[0], ny = y+dir[1];   // Position after moving.
                    if (in_board(nx, ny)) set_bit(board, (ny<<3) + nx);
                }
            }
            if (bit(kings, i)) {
                for (const auto& dir: DIR_K) {
                    const char nx = x+dir[0], ny = y+dir[1];
                    if (in_board(nx, ny)) set_bit(board, (ny<<3) + nx);
                }
            }
            if (bit(rooks, i) || bit(queens, i)) {
                for (const auto& dir: DIR_R) {
                    char cx = x, cy = y;                  // Current (x, y)
                    const char dx = dir[0], dy = dir[1];  // Delta (x, y)
                    while (true) {
                        cx += dx;
                        cy += dy;
                        if (!in_board(cx, cy)) break;
                        const char loc = (cy<<3) + cx;
                        set_bit(board, loc);
                        if (bit(pieces, loc)) break;
                    }
                }
            }
            if (bit(bishops, i) || bit(queens, i)) {
                for (const auto& dir: DIR_B) {
                    char cx = x, cy = y;                  // Current (x, y)
                    const char dx = dir[0], dy = dir[1];  // Delta (x, y)
                    while (true) {
                        cx += dx;
                        cy += dy;
                        if (!in_board(cx, cy)) break;
                        const char loc = (cy<<3) + cx;
                        set_bit(board, loc);
                        if (bit(pieces, loc)) break;
                    }
                }
            }
        }

        return board;
    }

    U64 attacked(const Position& pos, const bool& turn) {
        /*
        Wrapper for attacked.
        pos: Position.
        turn: The side that is attacking.
        return: Bitboard of attacks.
        */
        if (turn) {
            const U64 opp = pos.bp | pos.bn | pos.bb | pos.br | pos.bq;
            return attacked(pos.wp, pos.wn, pos.wb, pos.wr, pos.wq, pos.wk, opp, true);
        } else {
            const U64 opp = pos.wp | pos.wn | pos.wb | pos.wr | pos.wq;
            return attacked(pos.bp, pos.bn, pos.bb, pos.br, pos.bq, pos.bk, opp, false);
        }
    }

    char num_attacks(const vector<Move>& moves, const Location& sq) {
        // Returns the number of attackers attacking a certain square.

        char cnt = 0;
        const char loc = (sq.y<<3) + sq.x;
        for (const auto& move: moves) {
            if (move.to == loc) cnt++;
        }
        return cnt;
    }

    U64 pinned(const Location& k_pos, const Location& piece_pos, const U64& pawns, const U64& knights, const U64& bishops,
            const U64& rooks, const U64& queens, const U64& same) {
        /*
        Calculates a bitboard of all squares the piece at piece_pos can move to.
        If the piece is not pinned, it will return FULL.
        k_pos: King pos of current side.
        piece_pos: Position of piece to check if pinned.
        pawns, knights, ...: Enemy bitboards.
        same: Bitboard of all same side pieces.
        */
        U64 pin_ray = EMPTY;
        const U64 all = pawns | knights | bishops | rooks | queens | same;
        const char kx = k_pos.x, ky = k_pos.y;
        const char px = piece_pos.x, py = piece_pos.y;
        char dx = px - kx, dy = py - ky;
        bool found = false;  // true = piece is pinned, false = piece is not pinned.

        if (dx > 0) dx = 1;
        else if (dx < 0) dx = -1;
        if (dy > 0) dy = 1;
        else if (dy < 0) dy = -1;

        if (!(dx == 0 && dy == 0) && (dx == 0 && dy != 0) || (dx != 0 && dy == 0)) {
            char cx = kx, cy = ky;   // Current (x, y)
            while (true) {
                cx += dx;
                cy += dy;
                if (!in_board(cx, cy)) {
                    found = false;
                    break;
                }
                const char loc = (cy<<3) + cx;
                set_bit(pin_ray, loc);
                if (cx==px && cy==py) found = true;
                else if (bit(rooks, loc) || bit(queens, loc)) break;
                else if (bit(all, loc)) {
                    found = false;
                    break;
                }
            }
        } else if (!(dx == 0 && dy == 0) && (abs(dx) == abs(dy))) {
            char cx = kx, cy = ky;   // Current (x, y)
            while (true) {
                cx += dx;
                cy += dy;
                if (!in_board(cx, cy)) {
                    found = false;
                    break;
                }
                const char loc = (cy<<3) + cx;
                set_bit(pin_ray, loc);
                if (cx==px && cy==py) found = true;
                else if (bit(bishops, loc) || bit(queens, loc)) break;
                else if (bit(all, loc)) {
                    found = false;
                    break;
                }
            }
        }

        return (found ? pin_ray : FULL);
    }

    U64 checkers(const Location& k_pos, const U64& pawns, const U64& knights, const U64& bishops,
            const U64& rooks, const U64& queens, const U64& same_side, const U64& attackers, const bool& side) {
        /*
        Calculates bitboard of checking pieces.
        k_pos: King position.
        pawns, knights, ...: Boards of enemy pieces.
        same_side: Same side pieces.
        attackers: Attacks of enemy.
        side: true if white else false.
        */
        U64 board = EMPTY;
        char atk_cnt = 0;  // Attacker count, can also be thought of as number of attackers.
        const char kx = k_pos.x, ky = k_pos.y;
        if (!bit(attackers, (ky<<3)+kx)) return board;
        const U64 pieces = pawns | knights | bishops | rooks | queens;
        const char pawn_dir = side ? -1 : 1;

        // Pawns
        const char y = ky - pawn_dir;
        if (0 <= y && y < 8) {
            const char loc = (y<<3) + kx;
            if (0 <= kx-1 && kx-1 < 8) {
                if (bit(pawns, loc-1)) {
                    atk_cnt++;
                    set_bit(board, loc-1);
                }
            }
            if (0 <= kx+1 && kx+1 < 8) {
                if (bit(pawns, loc+1)) {
                    atk_cnt++;
                    set_bit(board, loc+1);
                }
            }
        }

        // Knights
        for (const auto& dir: DIR_N) {
            const char x = kx + dir[0], y = ky + dir[1];
            if (in_board(x, y)) {
                const char loc = (y<<3) + x;
                if (bit(knights, loc)) {
                    set_bit(board, loc);
                    if (atk_cnt++ > 1) return board;
                    break;
                }
            }
        }

        // Bishops and part queens
        for (const auto& dir: DIR_B) {
            char x = kx, y = ky;
            while (true) {
                x += dir[0];
                y += dir[1];
                if (!in_board(x, y)) break;
                const char loc = (y<<3) + x;
                if (bit(same_side, loc)) break;
                if (bit(bishops, loc) || bit(queens, loc)) {
                    set_bit(board, loc);
                    if (atk_cnt++ > 1) return board;
                    break;
                }
                if (bit(pieces, loc)) break;
            }
        }

        // Rooks and part queens
        for (const auto& dir: DIR_R) {
            char x = kx, y = ky;
            while (true) {
                x += dir[0];
                y += dir[1];
                if (!in_board(x, y)) break;
                const char loc = (y<<3) + x;
                if (bit(same_side, loc)) break;
                if (bit(rooks, loc) || bit(queens, loc)) {
                    set_bit(board, loc);
                    if (atk_cnt++ > 1) return board;
                    break;
                }
                if (bit(pieces, loc)) break;
            }
        }

        return board;
    }

    void king_moves(Move* moves, int& movecnt, const Location& k_pos, const char& castling, const bool& side, const U64& same,
            const U64& all, const U64& attacks) {
        /*
        Calculates all king moves.
        k_pos: King position.
        castling: Castling rights.
        side: true if white else false
        same: board of same pieces.
        all: board of all pieces.
        attacks: attacks from enemy.
        */
        const char kx = k_pos.x, ky = k_pos.y;
        const char start = (ky<<3) + kx;

        for (const auto& dir: DIR_K) {
            const char x = kx+dir[0], y = ky+dir[1];
            if (in_board(x, y)) {
                const char loc = (y<<3) + x;
                if (!bit(attacks, loc) && !bit(same, loc)) {
                    moves[movecnt++] = Move(start, loc);
                }
            }
        }

        // Castling
        if (side) {
            if (bit(castling, 0)) {
                if (!bit(all, 5) && !bit(all, 6)) {
                    if ((CASTLING_WK & attacks) == EMPTY) {
                        moves[movecnt++] = Move(start, 6);
                    }
                }
            }
            if (bit(castling, 1)) {
                if (!bit(all, 1) && !bit(all, 2) && !bit(all, 3)) {
                    if ((CASTLING_WQ & attacks) == EMPTY) {
                        moves[movecnt++] = Move(start, 2);
                    }
                }
            }
        } else {
            if (bit(castling, 2)) {
                if (!bit(all, 61) && !bit(all, 62)) {
                    if ((CASTLING_BK & attacks) == EMPTY) {
                        moves[movecnt++] = Move(start, 62);
                    }
                }
            }
            if (bit(castling, 3)) {
                if (!bit(all, 57) && !bit(all, 58) && !bit(all, 59)) {
                    if ((CASTLING_BQ & attacks) == EMPTY) {
                        moves[movecnt++] = Move(start, 58);
                    }
                }
            }
        }
    }

    void single_check_moves(Move* moves, int& movecnt, const Position& pos, const U64& CP, const U64& CN, const U64& CB, const U64& CR,
            const U64& CQ, const U64& OP, const U64& ON, const U64& OB, const U64& OR, const U64& OQ, const U64& OK, const U64& SAME,
            const U64& OPPONENT, const U64& ALL, const Location& k_pos, const U64& checking_pieces) {
        /*
        Computes all moves when there is one check.
        */
        // Block and capture piece giving check to king
        U64 block_mask = EMPTY;
        const U64 capture_mask = checking_pieces;
        const char pawn_dir = pos.turn ? 1 : -1;

        const Location check_pos = first_bit(checking_pieces);
        const char check_x = check_pos.x, check_y = check_pos.y;

        const char kx = k_pos.x, ky = k_pos.y;
        char dx = check_x - kx, dy = check_y - ky;
        // If not a knight move create the mask
        if (!((abs(dx)==2 && abs(dy)==1) || (abs(dy)==2 && abs(dx)==1))) {
            if (dx > 0) dx = 1;
            else if (dx < 0) dx = -1;
            if (dy > 0) dy = 1;
            else if (dy < 0) dy = -1;

            char cx = kx, cy = ky;   // Current (x, y)
            while (true) {
                cx += dx;
                cy += dy;
                const char loc = (cy<<3) + cx;
                if (bit(checking_pieces, loc)) break;
                set_bit(block_mask, loc);
            }
        }

        const U64 full_mask = block_mask | capture_mask;

        // Go through all pieces and check if they can capture/block
        for (char i = 0; i < 64; i++) {
            const Location curr_loc = Location(i&7, (i>>3));
            if (bit(SAME, i) && pinned(k_pos, curr_loc, OP, OK, OB, OR, OQ, SAME) == FULL) {
                if (bit(CP, i)) {
                    const char x = (i&7);
                    char y;

                    // Block
                    y = (i>>3);
                    const char speed = (y == (pos.turn ? 1 : 6)) ? 2 : 1;  // If white check rank 6 else rank 1 if on that rank 2 else 1
                    if (pos.turn) {
                        for (char cy = y + 1; cy < y + speed + 1; cy++) {
                            const char loc = (cy<<3) + x;
                            if (bit(ALL, loc)) break;
                            if (bit(block_mask, loc)) {
                                if (cy == 7) {
                                    // Promotion
                                    for (const char& p: {0, 1, 2, 3}) {
                                        moves[movecnt++] = Move(i, loc, true, p);
                                    }
                                } else {
                                    moves[movecnt++] = Move(i, loc);
                                }
                                break;
                            }
                        }
                    } else {
                        for (char cy = y - 1; cy > y - speed - 1; cy--) {
                            const char loc = (cy<<3) + x;
                            if (bit(ALL, loc)) break;
                            if (bit(block_mask, loc)) {
                                if (cy == 0) {
                                    // Promotion
                                    for (const char& p: {0, 1, 2, 3}) {
                                        moves[movecnt++] = Move(i, loc, true, p);
                                    }
                                } else {
                                    moves[movecnt++] = Move(i, loc);
                                }
                                break;
                            }
                        }
                    }
                    // Capture
                    U64 new_capture = capture_mask;
                    if (pos.ep) {
                        char start, end;
                        const char ep_x = pos.ep_square % 8;
                        if (pos.turn) {
                            start = 4;
                            end = 5;
                        } else {
                            start = 3;
                            end = 2;
                        }
                        if (y == start && bit(OP, (start<<3) + ep_x)) set_bit(new_capture, (end<<3) + ep_x);
                    }
                    y += pawn_dir;
                    if (0 <= y && y < 8) {
                        const bool promo = y == 7 || y == 0;
                        for (const auto& offset: {x-1, x+1}) {
                            if (0 <= offset && offset < 8) {
                                const char char_move = (y<<3) + offset;
                                if (bit(new_capture, char_move) && (bit(OPPONENT, char_move) || ((char_move == pos.ep_square) && pos.ep))) {
                                    if (promo) {
                                        for (const char& p: {0, 1, 2, 3}) {
                                            moves[movecnt++] = Move(i, char_move, true, p);
                                        }
                                    } else {
                                        moves[movecnt++] = Move(i, char_move);
                                    }
                                }
                            }
                        }
                    }
                } else if (bit(CN, i)) {
                    const char x = (i&7), y = (i>>3);
                    for (const auto& dir: DIR_N) {
                        char cx = x + dir[0], cy = y + dir[1];   // Current (x, y)
                        if (!in_board(cx, cy)) continue;
                        const char loc = (cy<<3) + cx;
                        if (bit(full_mask, loc)) {
                            moves[movecnt++] = Move(i, loc);
                        }
                    }
                } else if (bit(CB, i) || bit(CQ, i)) {
                    // Capture and block
                    for (const auto& dir: DIR_B) {
                        char cx = (i&7), cy = (i>>3);             // Current (x, y)
                        const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                        while (true) {
                            cx += dx;
                            cy += dy;
                            if (!in_board(cx, cy)) break;
                            const char loc = (cy<<3) + cx;
                            if (bit(SAME, loc)) break;
                            if (bit(full_mask, loc)) {
                                moves[movecnt++] = Move(i, loc);
                                break;
                            }
                            if (bit(OPPONENT, loc)) break;
                        }
                    }
                }
                if (bit(CR, i) || bit(CQ, i)) {
                    // Capture and block
                    for (const auto& dir: DIR_R) {
                        char cx = (i&7), cy = (i>>3);             // Current (x, y)
                        const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                        while (true) {
                            cx += dx;
                            cy += dy;
                            if (!in_board(cx, cy)) break;
                            const char loc = (cy<<3) + cx;
                            if (bit(SAME, loc)) break;
                            if (bit(full_mask, loc)) {
                                moves[movecnt++] = Move(i, loc);
                                break;
                            }
                            if (bit(OPPONENT, loc)) break;
                        }
                    }
                }
            }
        }
    }

    void no_check_moves(Move* moves, int& movecnt, const Position& pos, const U64& CP, const U64& CN, const U64& CB, const U64& CR,
            const U64& CQ, const U64& OP, const U64& ON, const U64& OB, const U64& OR, const U64& OQ, const U64& OK, const U64& SAME,
            const U64& OPPONENT, const U64& ALL, const Location& k_pos, const U64& checking_pieces) {
        /*
        Computes all moves when there is no check.
        */
        const char pawn_dir = pos.turn ? 1 : -1;
        for (auto i = 0; i < 64; i++) {
            const Location curr_loc = Location(i&7, (i>>3));
            if (bit(SAME, i)) {
                U64 pin = pinned(k_pos, curr_loc, OP, OK, OB, OR, OQ, SAME);
                bool piece_pinned = (pin != FULL);

                if (bit(CP, i)) {
                    const char x = (i&7);
                    char y = i>>3;

                    // Forward
                    const char speed = (y == (pos.turn ? 1 : 6)) ? 2 : 1;  // Set speed to 2 if pawn's first move.
                    if (pos.turn) {
                        for (char cy = y + 1; cy < y + speed + 1; cy++) {
                            const char loc = (cy<<3) + x;
                            if (bit(ALL, loc)) break;
                            if (bit(pin, loc)) {
                                if (cy == 7) {
                                    // Promotion
                                    for (const char& p: {0, 1, 2, 3}) {
                                        moves[movecnt++] = Move(i, loc, true, p);
                                    }
                                } else {
                                    moves[movecnt++] = Move(i, loc);
                                }
                            }
                        }
                    } else {
                        for (char cy = y - 1; cy > y - speed - 1; cy--) {
                            const char loc = (cy<<3) + x;
                            if (bit(ALL, loc)) break;
                            if (bit(pin, loc)) {
                                if (cy == 0) {
                                    // Promotion
                                    for (const char& p: {0, 1, 2, 3}) {
                                        moves[movecnt++] = Move(i, loc, true, p);
                                    }
                                } else {
                                    moves[movecnt++] = Move(i, loc);
                                }
                            }
                        }
                    }
                    // Captures
                    y += pawn_dir;
                    if (0 <= y && y < 8) {
                        const bool promo = (y == 0) || (y == 7);
                        for (const auto& offset: {x-1, x+1}) {
                            if (0 <= offset && offset < 8) {
                                const char char_move = (y<<3) + offset;
                                if ((bit(pin, char_move) && (bit(OPPONENT, char_move) || ((char_move == pos.ep_square) && pos.ep)))) {
                                    if (promo) {
                                        for (const char& p: {0, 1, 2, 3}) {
                                            moves[movecnt++] = Move(i, char_move, true, p);
                                        }
                                    } else {
                                        moves[movecnt++] = Move(i, char_move);
                                    }
                                }
                            }
                        }
                    }
                } else if (bit(CN, i)) {
                    // Knights cannot move while pinned.
                    if (piece_pinned) continue;
                    else {
                        for (const auto& dir: DIR_N) {
                            const char x = (i&7) + dir[0], y = (i>>3) + dir[1];   // Current (x, y)
                            if (!in_board(x, y)) continue;
                            const char loc = (y<<3) + x;
                            if (!bit(SAME, loc)) {
                                moves[movecnt++] = Move(i, loc);
                            }
                        }
                    }
                } else if (bit(CB, i) || bit(CQ, i)) {
                    for (const auto& dir: DIR_B) {
                        char cx = (i&7), cy = (i>>3);             // Current (x, y)
                        const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                        while (true) {
                            cx += dx;
                            cy += dy;
                            if (!in_board(cx, cy)) break;
                            const char loc = (cy<<3) + cx;
                            if (bit(SAME, loc)) break;
                            if (bit(pin, loc)) {
                                moves[movecnt++] = Move(i, loc);
                            }
                            if (bit(OPPONENT, loc)) break;
                        }
                    }
                }
                if (bit(CR, i) || bit(CQ, i)) {
                    for (const auto& dir: DIR_R) {
                        char cx = (i&7), cy = (i>>3);             // Current (x, y)
                        const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                        while (true) {
                            cx += dx;
                            cy += dy;
                            if (!in_board(cx, cy)) break;
                            const char loc = (cy<<3) + cx;
                            if (bit(SAME, loc)) break;
                            if (bit(pin, loc)) {
                                moves[movecnt++] = Move(i, loc);
                            }
                            if (bit(OPPONENT, loc)) break;
                        }
                    }
                }
            }
        }
    }

    vector<Move> legal_moves(const Position pos, const U64& attacks) {
        // Pass in attacks from opponent.
        // Current and opponent pieces and sides
        U64 CP, CN, CB, CR, CQ, CK, OP, ON, OB, OR, OQ, OK;
        if (pos.turn) {
            CP = pos.wp;
            CN = pos.wn;
            CB = pos.wb;
            CR = pos.wr;
            CQ = pos.wq;
            CK = pos.wk;
            OP = pos.bp;
            ON = pos.bn;
            OB = pos.bb;
            OR = pos.br;
            OQ = pos.bq;
            OK = pos.bk;
        } else {
            CP = pos.bp;
            CN = pos.bn;
            CB = pos.bb;
            CR = pos.br;
            CQ = pos.bq;
            CK = pos.bk;
            OP = pos.wp;
            ON = pos.wn;
            OB = pos.wb;
            OR = pos.wr;
            OQ = pos.wq;
            OK = pos.wk;
        }
        const U64 SAME = CP | CN | CB | CR | CQ | CK;
        const U64 OPPONENT = OP | ON | OB | OR | OQ | OK;
        const U64 ALL = SAME | OPPONENT;

        const Location k_pos = first_bit(CK);
        const char kx = k_pos.x, ky = k_pos.y;
        const U64 checking_pieces = checkers(k_pos, OP, ON, OB, OR, OQ, SAME, attacks, pos.turn);
        const char num_checkers = popcnt(checking_pieces);
        const char pawn_dir = pos.turn ? 1 : -1;

        int movecnt = 0;
        Move moves[MAX_MOVES];
        if (num_checkers == 0) {
            no_check_moves(moves, movecnt, pos, CP, CN, CB, CR, CQ, OP, ON, OB, OR, OQ, OK, SAME, OPPONENT, ALL, k_pos, checking_pieces);
        } else if (num_checkers == 1) {
            single_check_moves(moves, movecnt, pos, CP, CN, CB, CR, CQ, OP, ON, OB, OR, OQ, OK, SAME, OPPONENT, ALL, k_pos, checking_pieces);
        }
        king_moves(moves, movecnt, k_pos, pos.castling, pos.turn, SAME, ALL, attacks);
        return vector<Move>(moves, moves+movecnt);
    }


    Position startpos() {
        Position pos;
        pos.wp = START_WP;
        pos.wn = START_WN;
        pos.wb = START_WB;
        pos.wr = START_WR;
        pos.wq = START_WQ;
        pos.wk = START_WK;
        pos.bp = START_BP;
        pos.bn = START_BN;
        pos.bb = START_BB;
        pos.br = START_BR;
        pos.bq = START_BQ;
        pos.bk = START_BK;
        pos.turn = true;
        pos.castling = 15;
        pos.ep = false;

        return pos;
    }

    Position push(Position pos, const Move& move) {
        U64* pointers[12] = {&pos.wp, &pos.wn, &pos.wb, &pos.wr, &pos.wq, &pos.wk,
            &pos.bp, &pos.bn, &pos.bb, &pos.br, &pos.bq, &pos.bk};
        U64* to_board = pointers[0];
        bool is_king = false;
        bool is_pawn = false;

        // Find to_board and set bits.
        for (char i = 0; i < 12; i++) {
            U64* p = pointers[i];
            if (bit(*p, move.from)) {
                to_board = p;
                if ((i == 5) || (i == 11)) is_king = true;
                if ((i == 0) || (i == 6)) is_pawn = true;
                unset_bit(*p, move.from);
            }
            unset_bit(*p, move.to);
        }
        if (move.is_promo) {
            if (pos.turn) {
                switch (move.promo) {
                    case 0: to_board = &pos.wn; break;
                    case 1: to_board = &pos.wb; break;
                    case 2: to_board = &pos.wr; break;
                    case 3: to_board = &pos.wq; break;
                }
            } else {
                switch (move.promo) {
                    case 0: to_board = &pos.bn; break;
                    case 1: to_board = &pos.bb; break;
                    case 2: to_board = &pos.br; break;
                    case 3: to_board = &pos.bq; break;
                }
            }
        }
        set_bit(*to_board, move.to);

        // Castling
        if (is_king && (abs(move.to-move.from) == 2)) {
            const char x = (move.to > move.from) ? 7 : 0;
            const char new_x = (move.to > move.from) ? 5 : 3;
            if (move.from < 8) {
                unset_bit(*pointers[3], x);
                set_bit(*pointers[3], new_x);
            } else {
                unset_bit(*pointers[9], x+56);
                set_bit(*pointers[9], new_x+56);
            }
            pos.castling = 0;
        } else {
            switch (move.from) {
                case  0: unset_bit(pos.castling, 1);                             break;
                case  4: unset_bit(pos.castling, 0); unset_bit(pos.castling, 1); break;
                case  7: unset_bit(pos.castling, 0);                             break;
                case 56: unset_bit(pos.castling, 3);                             break;
                case 60: unset_bit(pos.castling, 2); unset_bit(pos.castling, 3); break;
                case 63: unset_bit(pos.castling, 2);                             break;
            }
            switch (move.to) {
                case  0: unset_bit(pos.castling, 1); break;
                case  7: unset_bit(pos.castling, 0); break;
                case 56: unset_bit(pos.castling, 3); break;
                case 63: unset_bit(pos.castling, 2); break;
            }
        }

        // En passant
        const bool ep = pos.ep;
        pos.ep = false;
        if (is_pawn) {
            if (move.to > move.from) {
                if ((move.to-move.from) == 16) {
                    pos.ep = true;
                    pos.ep_square = move.from + 8;
                } else if (ep && (((move.to-move.from) == 7) || ((move.to-move.from) == 9))) {
                    unset_bit(pos.bp, move.to-8);
                }
            } else {
                if ((move.from-move.to) == 16) {
                    pos.ep = true;
                    pos.ep_square = move.from - 8;
                } else if (ep && (((move.from-move.to) == 7) || ((move.from-move.to) == 9))) {
                    unset_bit(pos.wp, move.to+8);
                }
            }
        }

        pos.turn = !pos.turn;
        pos.move_cnt++;
        return pos;
    }

    Position push(Position pos, string uci) {
        return push(pos, parse_uci(uci));
    }
}
