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
#include "options.hpp"

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

    active = true;
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

    active = true;
}


Location::Location() {
    x = 0;
    y = 0;
}

Location::Location(char _x, char _y) {
    x = _x;
    y = _y;
}


namespace Bitboard {
    bool bit(U64 board, int pos) {
        return ((1ULL << pos) & board) != 0;
    }

    bool bit(char board, int pos) {
        return ((1ULL << pos) & board) != 0;
    }

    char popcnt(U64 num) {
        U64 mask = 255ULL;
        char count = 0;
        count += popcnt_tbl[num&mask];
        count += popcnt_tbl[(num>>8)&mask];
        count += popcnt_tbl[(num>>16)&mask];
        count += popcnt_tbl[(num>>24)&mask];
        count += popcnt_tbl[(num>>32)&mask];
        count += popcnt_tbl[(num>>40)&mask];
        count += popcnt_tbl[(num>>48)&mask];
        count += popcnt_tbl[(num>>56)&mask];
        return count;
    }

    void set_bit(U64& board, int pos) {
        board |= (1ULL << pos);
    }

    void unset_bit(U64& board, int pos) {
        board &= ~(1ULL << pos);
    }

    void set_bit(char& board, int pos) {
        board |= (1ULL << pos);
    }

    void unset_bit(char& board, int pos) {
        board &= ~(1ULL << pos);
    }

    bool contains(vector<vector<char>> sequence, vector<char> target) {
        for (auto i: sequence) {
            if (i == target) return true;
        }
        return false;
    }

    Location first_bit(U64 board) {
        char pos = log2(board & -board);
        return Location(pos%8, pos/8);
    }


    string piece_at(Position pos, char loc) {
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

    string board_str(U64 board, string on, string off) {
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

    string board_str(Position pos) {
        const string row = " +---+---+---+---+---+---+---+---+";
        const string col = " | ";
        string str = "";

        str += row + "\n";
        for (auto y = 7; y >= 0; y--) {
            for (auto x = 0; x < 8; x++) {
                int loc = 8*y + x;
                string symbol = piece_at(pos, loc);
                str += col;
                str += symbol;
            }
            str += col + "\n" + row + "\n";
        }
        str += "\nFen: " + fen(pos) + "\n";

        return str;
    }

    string square_str(char sq) {
        char x = sq%8, y = sq/8;
        return string(1, x+97) + std::to_string(y+1);
    }

    string move_str(Move move) {
        string str;
        str += square_str(move.from);
        str += square_str(move.to);
        if (move.is_promo) {
            switch (move.promo) {
                case 0: str += "N"; break;
                case 1: str += "B"; break;
                case 2: str += "R"; break;
                case 3: str += "Q"; break;
            }
        }
        return str;
    }

    string fen(Position pos) {
        string str = "";
        int blank_count = 0;

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

        vector<bool> c = {bit(pos.castling, 0), bit(pos.castling, 1), bit(pos.castling, 2), bit(pos.castling, 3)};
        if (!(c[0] || c[1] || c[2] || c[3])) str += "-";
        else {
            if (c[0]) str += "K";
            if (c[1]) str += "Q";
            if (c[2]) str += "k";
            if (c[3]) str += "q";
        }
        str += " ";

        if (pos.ep) {
            str += square_str(pos.ep_square);
        } else {
            str += "-";
        }
        str += " ";

        str += "0 1";

        return str;
    }

    Position parse_fen(string fen) {
        vector<string> parts = split(fen, " ");
        Position pos;

        int x = 0, y = 7;
        for (auto ch: parts[0]) {
            if (ch == '/') {
                y--;
                x = 0;
            } else {
                int loc = 8*y + x;
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

        int castling = 0;
        for (auto ch: parts[2]) {
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
            pos.ep_square = std::stoi(string(1, parts[3][1]))*8 + (parts[3][0] - 97);
        }

        return pos;
    }

    Move parse_uci(string uci) {
        Move move;
        move.from = uci[0]-97 + 8*(uci[1]-49);
        move.to = uci[2]-97 + 8*(uci[3]-49);
        move.is_promo = (uci.size() >= 5);
        if (move.is_promo) {
            switch (uci[4]) {
                case 'N': move.promo = 0; break; case 'n': move.promo = 0; break;
                case 'B': move.promo = 1; break; case 'b': move.promo = 1; break;
                case 'R': move.promo = 2; break; case 'r': move.promo = 2; break;
                case 'Q': move.promo = 3; break; case 'q': move.promo = 3; break;
            }
        }
        return move;
    }


    U64 attacked(U64 pawns, U64 knights, U64 bishops, U64 rooks, U64 queens, U64 kings, U64 opponent, bool side) {
        const U64 pieces = pawns | knights | bishops | rooks | queens | kings;
        const char pawn_dir = side ? 1 : -1;
        U64 board = EMPTY;

        for (char i = 0; i < 64; i++) {
            const char x = i%8, y = i/8;
            if (bit(pawns, i)) {
                const char ny = y + pawn_dir;  // Current (x, y) with y as after capture.
                if (0 <= ny && ny < 8) {
                    if (0 <= x-1 && x-1 < 8) set_bit(board, ny*8 + x-1);
                    if (0 <= x+1 && x+1 < 8) set_bit(board, ny*8 + x+1);
                }
            }
            if (bit(knights, i)) {
                for (auto dir: DIR_N) {                        // Iterate through all knight moves.
                    const char nx = x+dir[0], ny = y+dir[1];   // Position after moving.
                    if (0 <= nx && nx < 8 && 0 <= ny && ny < 8) set_bit(board, ny*8 + nx);
                }
            }
            if (bit(kings, i)) {
                for (auto dir: DIR_K) {
                    const char kx = x+dir[0], ky = y+dir[1];
                    if (0 <= kx && kx < 8 && 0 <= ky && ky < 8) set_bit(board, ky*8 + kx);
                }
            }
            if (bit(rooks, i) || bit(queens, i)) {
                for (auto dir: DIR_R) {
                    char cx = x, cy = y;                  // Current (x, y)
                    const char dx = dir[0], dy = dir[1];  // Delta (x, y)
                    while (true) {
                        cx += dx;
                        cy += dy;
                        if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                        const char loc = cy*8 + cx;
                        set_bit(board, loc);
                        if (bit(opponent, loc)) break;
                        if (bit(pieces, loc)) break;
                    }
                }
            }
            if (bit(bishops, i) || bit(queens, i)) {
                for (auto dir: DIR_B) {
                    char cx = x, cy = y;                  // Current (x, y)
                    const char dx = dir[0], dy = dir[1];  // Delta (x, y)
                    while (true) {
                        cx += dx;
                        cy += dy;
                        if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                        const char loc = cy*8 + cx;
                        set_bit(board, loc);
                        if (bit(opponent, loc)) break;
                        if (bit(pieces, loc)) break;
                    }
                }
            }
        }

        return board;
    }

    U64 attacked(Position pos, bool turn) {
        if (turn) {
            U64 opp = pos.bp | pos.bn | pos.bb | pos.br | pos.bq | pos.bk;
            return attacked(pos.wp, pos.wn, pos.wb, pos.wr, pos.wq, pos.wk, opp, true);
        } else {
            U64 opp = pos.wp | pos.wn | pos.wb | pos.wr | pos.wq | pos.wk;
            return attacked(pos.bp, pos.bn, pos.bb, pos.br, pos.bq, pos.bk, opp, false);
        }
    }

    tuple<bool, U64> pinned(U64 king, U64 piece, U64 pawns, U64 knights, U64 bishops, U64 rooks, U64 queens, U64 same) {
        const U64 opponent = pawns | knights | bishops | rooks | queens;
        const Location k_pos = first_bit(king);
        const char kx = k_pos.x, ky = k_pos.y;
        U64 pin_ray = EMPTY;
        bool found;

        for (auto dir: DIR_R) {
            found = false;
            char cx = kx, cy = ky;                  // Current (x, y)
            const char dx = dir[0], dy = dir[1];    // Delta (x, y)
            while (true) {
                cx += dx;
                cy += dy;
                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                const char loc = cy*8 + cx;
                set_bit(pin_ray, loc);
                if (bit(rooks, loc) || bit(queens, loc)) {
                    if (found) {
                        return tuple<bool, U64>(true, pin_ray);
                    } else {
                        return tuple<bool, U64>(false, FULL);
                    }
                }
                else if (bit(opponent, loc)) return tuple<bool, U64>(false, FULL);
                else if (bit(piece, loc)) found = true;
                else if (bit(same, loc)) return tuple<bool, U64>(false, FULL);
            }
            if (found) return tuple<bool, U64>(false, FULL);
        }

        for (auto dir: DIR_B) {
            found = false;
            char cx = kx, cy = ky;                  // Current (x, y)
            const char dx = dir[0], dy = dir[1];    // Delta (x, y)
            while (true) {
                cx += dx;
                cy += dy;
                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                const char loc = cy*8 + cx;
                set_bit(pin_ray, loc);
                if (bit(rooks, loc) || bit(queens, loc)) {
                    if (found) {
                        return tuple<bool, U64>(true, pin_ray);
                    } else {
                        return tuple<bool, U64>(false, FULL);
                    }
                }
                else if (bit(opponent, loc)) return tuple<bool, U64>(false, FULL);
                else if (bit(piece, loc)) found = true;
                else if (bit(same, loc)) return tuple<bool, U64>(false, FULL);
            }
            if (found) return tuple<bool, U64>(false, FULL);
        }
        return tuple<bool, U64>(false, FULL);
    }

    tuple<U64, char> checkers(U64 king, U64 pawns, U64 knights, U64 bishops, U64 rooks, U64 queens, U64 same_side, U64 attackers, bool side) {
        U64 board = EMPTY;
        char atk_cnt = 0;  // Attacker count, can also be thought of as number of attackers.
        const Location k_pos = first_bit(king);
        const char kx = k_pos.x, ky = k_pos.y;
        if (!bit(attackers, ky*8+kx)) return tuple<U64, char>(board, atk_cnt);
        const U64 pieces = pawns | knights | bishops | rooks | queens;
        const char pawn_dir = side ? -1 : 1;

        // Pawns
        const char y = ky - pawn_dir;
        if (0 <= kx-1 && kx-1 < 8 && 0 <= y && y < 8) {
            const char loc = y*8 + kx-1;
            if (bit(pawns, loc)) {
                atk_cnt++;
                set_bit(board, loc);
            }
        }
        if (0 <= kx+1 && kx+1 < 8 && 0 <= y && y < 8) {
            const char loc = y*8 + kx+1;
            if (bit(pawns, loc)) {
                atk_cnt++;
                set_bit(board, loc);
            }
        }
        // Knights
        for (auto dir: DIR_N) {
            const char x = kx + dir[0], y = ky + dir[1];
            if (!(0 <= x && x < 8 && 0 <= y && y < 8)) break;
            const char loc = y*8 + x;
            if (bit(knights, loc)) {
                set_bit(board, loc);
                if (atk_cnt++ > 1) return tuple<U64, char>(board, atk_cnt);
                break;
            }
        }
        // Bishops and part queens
        for (auto dir: DIR_B) {
            char x = kx, y = ky;
            while (true) {
                x += dir[0];
                y += dir[1];
                if (!(0 <= x && x < 8 && 0 <= y && y < 8)) break;
                const char loc = y*8 + x;
                if (bit(same_side, loc)) break;
                if (bit(bishops, loc) || bit(queens, loc)) {
                    set_bit(board, loc);
                    if (atk_cnt++ > 1) return tuple<U64, char>(board, atk_cnt);
                    break;
                }
                if (bit(pieces, loc)) break;
            }
        }
        // Rooks and part queens
        for (auto dir: DIR_R) {
            char x = kx, y = ky;
            while (true) {
                x += dir[0];
                y += dir[1];
                if (!(0 <= x && x < 8 && 0 <= y && y < 8)) break;
                const char loc = y*8 + x;
                if (bit(same_side, loc)) break;
                if (bit(rooks, loc) || bit(queens, loc)) {
                    set_bit(board, loc);
                    if (atk_cnt++ > 1) return tuple<U64, char>(board, atk_cnt);
                    break;
                }
                if (bit(pieces, loc)) break;
            }
        }

        return tuple<U64, char>(board, atk_cnt);
    }

    vector<Move> king_moves(Position pos, U64 same, U64 all, U64 attacks) {
        // Pass in attacks from opponent.
        vector<Move> moves;
        Location k_pos = first_bit(pos.turn ? pos.wk : pos.bk);
        const char kx = k_pos.x, ky = k_pos.y;
        const char start = ky*8 + kx;

        for (auto dir: DIR_K) {
            const char x = kx+dir[0], y = ky+dir[1];
            if (0 <= x && x < 8 && 0 <= y && y < 8) {
                const char loc = y*8 + x;
                if (!bit(attacks, loc) && !bit(same, loc)) moves.push_back(Move(start, loc));
            }
        }

        // Castling
        if (pos.turn) {
            if (bit(pos.castling, 0)) {
                if (!bit(all, 5) && !bit(all, 6)) moves.push_back(Move(start, 6));
            }
            if (bit(pos.castling, 1)) {
                if (!bit(all, 1) && !bit(all, 2) && !bit(all, 3)) moves.push_back(Move(start, 2));
            }
        } else {
            if (bit(pos.castling, 2)) {
                if (!bit(all, 61) && !bit(all, 62)) moves.push_back(Move(start, 62));
            }
            if (bit(pos.castling, 3)) {
                if (!bit(all, 57) && !bit(all, 58) && !bit(all, 59)) moves.push_back(Move(start, 58));
            }
        }

        return moves;
    }

    vector<Move> legal_moves(Position pos, U64 attacks) {
        // Pass in attacks from opponent.
        // Current and opponent pieces and sides
        U64 CP, CN, CB, CR, CQ, CK, OP, ON, OB, OR, OQ, OK, SAME, OPPONENT, ALL;
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
        SAME = CP | CN | CB | CR | CQ | CK;
        OPPONENT = OP | ON | OB | OR | OQ | OK;
        ALL = SAME | OPPONENT;

        vector<Move> moves = king_moves(pos, SAME, ALL, attacks);
        const tuple<U64, char> checking_data = checkers(CK, OP, ON, OB, OR, OQ, SAME, attacks, pos.turn);
        const U64 checking_pieces = get<0>(checking_data);
        const char num_checkers = get<1>(checking_data);
        const char pawn_dir = pos.turn ? 1 : -1;

        if (num_checkers > 1) {
            return moves;
        } else if (num_checkers == 1) {
            // Block and capture piece giving check to king
            U64 block_mask = EMPTY, capture_mask = checking_pieces;
            Location k_pos = first_bit(CK), check_pos = first_bit(checking_pieces);
            const char kx = k_pos.x, ky = k_pos.y, check_x = check_pos.x, check_y = check_pos.y;

            char dx = check_x - kx, dy = check_y - ky;
            if (!contains(DIR_N, vector<char>({dx, dy}))) {
                if (dx != 0) dx /= abs(dx);
                if (dy != 0) dy /= abs(dy);
                char cx = kx, cy = ky;   // Current (x, y)
                while (true) {
                    cx += dx;
                    cy += dy;
                    const char loc = cy*8 + cx;
                    if (!bit(checking_pieces, loc)) set_bit(block_mask, loc);
                    else break;
                }
            }

            U64 full_mask = block_mask | capture_mask;

            // Go through all pieces and check if they can capture/block
            for (char i = 0; i < 64; i++) {
                if (bit(SAME, i) && !get<0>(pinned(CK, (1ULL << i), OP, OK, OB, OR, OQ, SAME))) {
                    if (bit(CP, i)) {
                        const char x = i%8;
                        char y;

                        // Block
                        y = i/8;
                        const char speed = (y == (pos.turn ? 1 : 6)) ? 2 : 1;  // If white check rank 6 else rank 1 if on that rank 2 else 1
                        if (pos.turn) {
                            for (auto cy = y + 1; cy < y + speed + 1; cy++) {
                                const char loc = cy*8 + x;
                                if (bit(ALL, loc)) break;
                                if (bit(block_mask, loc)) {
                                    if (cy == 7) {
                                        // Promotion
                                        for (char p: {0, 1, 2, 3}) {
                                            moves.push_back(Move(i, loc, true, p));
                                        }
                                    } else {
                                        moves.push_back(Move(i, loc));
                                    }
                                    break;
                                }
                            }
                        } else {
                            for (auto cy = y - 1; cy > y - speed - 1; cy--) {
                                const char loc = cy*8 + x;
                                if (bit(ALL, loc)) break;
                                if (bit(block_mask, loc)) {
                                    if (cy == 0) {
                                        // Promotion
                                        for (char p: {0, 1, 2, 3}) {
                                            moves.push_back(Move(i, loc, true, p));
                                        }
                                    } else {
                                        moves.push_back(Move(i, loc));
                                    }
                                    break;
                                }
                            }
                        }
                        // Capture
                        U64 new_capture = capture_mask;
                        if (pos.ep) {
                            char start, end, ep_x = pos.ep_square % 8;
                            if (pos.turn) {
                                start = 4;
                                end = 5;
                            } else {
                                start = 3;
                                end = 2;
                            }
                            if (y == start && bit(OP, start*8 + ep_x)) set_bit(new_capture, end*8 + ep_x);
                        }
                        y += pawn_dir;
                        if (0 <= y && y < 8) {
                            bool promo = y == 7 || y == 0;
                            for (auto offset: {x-1, x+1}) {
                                if (0 <= offset && offset < 8) {
                                    const char char_move = y*8 + offset;
                                    if (bit(new_capture, char_move) && (bit(OPPONENT, char_move) || char_move == pos.ep_square)) {
                                        if (promo) {
                                            for (char p: {0, 1, 2, 3}) {
                                                moves.push_back(Move(i, char_move, true, p));
                                            }
                                        } else {
                                            moves.push_back(Move(i, char_move));
                                        }
                                    }
                                }
                            }
                        }
                    } else if (bit(CN, i)) {
                        // Capture and block
                        char x = i%8, y = i/8;
                        for (auto dir: DIR_N) {
                            char cx = x + dir[0], cy = y + dir[1];   // Current (x, y)
                            if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                            const char loc = cy*8 + cx;
                            if (bit(full_mask, loc)) {
                                moves.push_back(Move(i, loc));
                                break;
                            }
                        }
                    } else if (bit(CB, i) || bit(CQ, i)) {
                        // Capture and block
                        for (auto dir: DIR_B) {
                            char cx = i%8, cy = i/8;                  // Current (x, y)
                            const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                            while (true) {
                                cx += dx;
                                cy += dy;
                                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                                const char loc = cy*8 + cx;
                                if (bit(SAME, loc)) break;
                                if (bit(full_mask, loc)) {
                                    moves.push_back(Move(i, loc));
                                    break;
                                }
                                if (bit(OPPONENT, loc)) break;
                            }
                        }
                    }
                    if (bit(CR, i) || bit(CQ, i)) {
                        // Capture and block
                        for (auto dir: DIR_R) {
                            char cx = i%8, cy = i/8;                  // Current (x, y)
                            const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                            while (true) {
                                cx += dx;
                                cy += dy;
                                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                                const char loc = cy*8 + cx;
                                if (bit(SAME, loc)) break;
                                if (bit(full_mask, loc)) {
                                    moves.push_back(Move(i, loc));
                                    break;
                                }
                                if (bit(OPPONENT, loc)) break;
                            }
                        }
                    }
                }
            }
        } else {
            for (auto i = 0; i < 64; i++) {
                if (bit(SAME, i)) {
                    tuple<bool, U64> pin = pinned(CK, (1ULL << i), OP, OK, OB, OR, OQ, SAME);
                    bool piece_pinned = get<0>(pin);
                    U64 pin_mask = get<1>(pin);

                    if (bit(CP, i)) {
                        const char x = i%8;
                        char y;

                        // Forward
                        y = i/8;
                        const char speed = (y == (pos.turn ? 1 : 6)) ? 2 : 1;  // Set speed to 2 if pawn's first move.
                        if (pos.turn) {
                            for (auto cy = y + 1; cy < y + speed + 1; cy++) {
                                const char loc = cy*8 + x;
                                if (bit(ALL, loc)) break;
                                if ((1ULL << loc) & pin_mask) {
                                    if (cy == 7) {
                                        // Promotion
                                        for (char p: {0, 1, 2, 3}) {
                                            moves.push_back(Move(i, loc, true, p));
                                        }
                                    } else {
                                        moves.push_back(Move(i, loc));
                                    }
                                }
                            }
                        } else {
                            for (auto cy = y - 1; cy > y - speed - 1; cy--) {
                                const char loc = cy*8 + x;
                                if (bit(ALL, loc)) break;
                                if ((1ULL << loc) & pin_mask) {
                                    if (cy == 7) {
                                        // Promotion
                                        for (char p: {0, 1, 2, 3}) {
                                            moves.push_back(Move(i, loc, true, p));
                                        }
                                    } else {
                                        moves.push_back(Move(i, loc));
                                    }
                                }
                            }
                        }
                        // Captures
                        y += pawn_dir;
                        if (0 <= y && y < 8) {
                            bool promo = y == 0 || y == 7;
                            for (auto offset: {x-1, x+1}) {
                                if (0 <= offset && offset < 8) {
                                    const char char_move = y*8 + offset;
                                    if ((bit(pin_mask, char_move) && (bit(OPPONENT, char_move) || char_move == pos.ep_square))) {
                                        if (promo) {
                                            for (char p: {0, 1, 2, 3}) {
                                                moves.push_back(Move(i, char_move, true, p));
                                            }
                                        } else {
                                            moves.push_back(Move(i, char_move));
                                        }
                                    }
                                }
                            }
                        }
                    } else if (bit(CN, i)) {
                        // Knights cannot move while pinned.
                        if (piece_pinned) {
                            continue;
                        } else {
                            for (auto dir: DIR_N) {
                                char x = i%8 + dir[0], y = i/8 + dir[1];   // Current (x, y)
                                if (!(0 <= x && x < 8 && 0 <= y && y < 8)) continue;
                                const char loc = y*8 + x;
                                if (!bit(SAME, loc)) moves.push_back(Move(i, loc));
                            }
                        }
                    } else if (bit(CB, i) || bit(CQ, i)) {
                        for (auto dir: DIR_B) {
                            char cx = i%8, cy = i/8;                  // Current (x, y)
                            const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                            while (true) {
                                cx += dx;
                                cy += dy;
                                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                                const char loc = cy*8 + cx;
                                if (bit(SAME, loc)) break;
                                if (((1ULL << loc) & pin_mask) != EMPTY) moves.push_back(Move(i, loc));
                                if (bit(OPPONENT, loc)) break;
                            }
                        }
                    }
                    if (bit(CR, i) || bit(CQ, i)) {
                        for (auto dir: DIR_R) {
                            char cx = i%8, cy = i/8;                  // Current (x, y)
                            const char dx = dir[0], dy = dir[1];      // Delta (x, y)
                            while (true) {
                                cx += dx;
                                cy += dy;
                                if (!(0 <= cx && cx < 8 && 0 <= cy && cy < 8)) break;
                                const char loc = cy*8 + cx;
                                if (bit(SAME, loc)) break;
                                if (((1ULL << loc) & pin_mask) != EMPTY) moves.push_back(Move(i, loc));
                                if (bit(OPPONENT, loc)) break;
                            }
                        }
                    }
                }
            }
        }
        return moves;
    }


    vector<U64*> bb_pointers(Position& pos) {
        U64* wp = &pos.wp;
        U64* wn = &pos.wn;
        U64* wb = &pos.wb;
        U64* wr = &pos.wr;
        U64* wq = &pos.wq;
        U64* wk = &pos.wk;
        U64* bp = &pos.bp;
        U64* bn = &pos.bn;
        U64* bb = &pos.bb;
        U64* br = &pos.br;
        U64* bq = &pos.bq;
        U64* bk = &pos.bk;
        return {wp, wn, wb, wr, wq, wk, bp, bn, bb, br, bq, bk};
    }

    Position startpos() {
        Position pos;
        pos.wp = Bitboard::START_WP;
        pos.wn = Bitboard::START_WN;
        pos.wb = Bitboard::START_WB;
        pos.wr = Bitboard::START_WR;
        pos.wq = Bitboard::START_WQ;
        pos.wk = Bitboard::START_WK;
        pos.bp = Bitboard::START_BP;
        pos.bn = Bitboard::START_BN;
        pos.bb = Bitboard::START_BB;
        pos.br = Bitboard::START_BR;
        pos.bq = Bitboard::START_BQ;
        pos.bk = Bitboard::START_BK;
        pos.turn = true;
        pos.castling = 15;
        pos.ep = false;

        return pos;
    }

    Position push(Position pos, Move move) {
        vector<U64*> pointers = bb_pointers(pos);
        U64* to_board = pointers[0];
        bool is_king = false;
        bool is_pawn = false;

        // Find to_board and set bits.
        for (auto i = 0; i < pointers.size(); i++) {
            U64* p = pointers[i];
            if (bit(*p, move.from)) {
                to_board = p;
                if ((i == 5) || (i == 11)) is_king = true;
                if ((i == 0) || (i == 6)) is_pawn = true;
            }
            unset_bit(*p, move.from);
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
            if (move.from == 0) {
                unset_bit(pos.castling, 1);
            } else if (move.from == 4) {
                unset_bit(pos.castling, 0);
                unset_bit(pos.castling, 1);
            } else if (move.from == 7) {
                unset_bit(pos.castling, 0);
            } else if (move.from == 56) {
                unset_bit(pos.castling, 3);
            } else if (move.from == 60) {
                unset_bit(pos.castling, 2);
                unset_bit(pos.castling, 3);
            } else if (move.from == 63) {
                unset_bit(pos.castling, 2);
            }
        }

        // En passant
        pos.ep = false;
        if (is_pawn) {
            if (move.to > move.from) {
                if ((move.to-move.from) == 16) {
                    pos.ep = true;
                    pos.ep_square = move.from + 8;
                } else if (((move.to-move.from) == 7) || ((move.to-move.from) == 9)) {
                    unset_bit(pos.bp, move.to-8);
                }
            } else {
                if ((move.from-move.to) == 16) {
                    pos.ep = true;
                    pos.ep_square = move.from - 8;
                } else if (((move.from-move.to) == 7) || ((move.from-move.to) == 9)) {
                    unset_bit(pos.wp, move.to+8);
                }
            }
        }

        pos.turn = !pos.turn;
        pos.move_stack.push_back(move);
        return pos;
    }

    Position push(Position pos, string uci) {
        return push(pos, parse_uci(uci));
    }
}
