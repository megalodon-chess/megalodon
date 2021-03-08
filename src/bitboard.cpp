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
#include "bitboard.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


namespace Bitboard {
    bool in_board(char x, char y) {
        return (0 <= x && x <= 7 && 0 <= y && y <= 7);
    }

    bool bit(long long board, int pos) {
        return ((1LL << pos) & board) != 0;
    }

    long long set_bit(long long board, int pos, bool value) {
        bool on = bit(board, pos);
        if (value && !on) {
            board += (1LL << pos);
        } else if (!value && on) {
            board -= (1LL << pos);
        }
        return board;
    }

    long long push(long long board, vector<char> move) {
        board = set_bit(board, move[0], false);
        board = set_bit(board, move[1], true);
        return board;
    }

    string board_str(long long board) {
        vector<string> rows;
        string final = "";

        for (auto y = 0; y < 8; y++) {
            string row = "";
            for (auto x = 0; x < 8; x++) {
                row += bit(board, 8*y+x) ? "1" : ".";
                row += " ";
            }
            rows.push_back(row);
        }

        for (auto i = 7; i >= 0; i--) {
            final += rows[i];
            final += "\n";
        }

        return final;
    }

    vector<vector<char>> legal_moves(long long wp, long long wn, long long wb, long long wr, long long wq, long long wk,
            long long bp, long long bn, long long bb, long long br, long long bq, long long bk, bool turn, char castling,
            bool ep, char ep_square) {
        if (turn) {
            vector<vector<char>> moves;
            long long same_col = wp | wn | wb | wr | wq | wk;
            long long diff_col = bp | bn | bb | br | bq | bk;

            vector<vector<char>> pmoves = pawn_moves(wp, same_col, diff_col, turn, ep, ep_square);
            vector<vector<char>> nmoves = knight_moves(wn, same_col, diff_col);
            vector<vector<char>> bmoves = bishop_moves(wb, same_col, diff_col);
            vector<vector<char>> rmoves = rook_moves(wr, same_col, diff_col);
            vector<vector<char>> qmoves = queen_moves(wq, same_col, diff_col);
            vector<vector<char>> kmoves = king_moves(wk, same_col, diff_col, castling);

            moves.insert(moves.end(), pmoves.begin(), pmoves.end());
            moves.insert(moves.end(), nmoves.begin(), nmoves.end());
            moves.insert(moves.end(), bmoves.begin(), bmoves.end());
            moves.insert(moves.end(), rmoves.begin(), rmoves.end());
            moves.insert(moves.end(), qmoves.begin(), qmoves.end());
            moves.insert(moves.end(), kmoves.begin(), kmoves.end());

            return moves;

        } else {
            vector<vector<char>> moves;
            long long same_col = bp | bn | bb | br | bq | bk;
            long long diff_col = wp | wn | wb | wr | wq | wk;

            vector<vector<char>> pmoves = pawn_moves(bp, same_col, diff_col, ep, ep_square);
            vector<vector<char>> nmoves = knight_moves(bn, same_col, diff_col);
            vector<vector<char>> bmoves = bishop_moves(bb, same_col, diff_col);
            vector<vector<char>> rmoves = rook_moves(br, same_col, diff_col);
            vector<vector<char>> qmoves = queen_moves(bq, same_col, diff_col);
            vector<vector<char>> kmoves = king_moves(bk, same_col, diff_col, castling);

            moves.insert(moves.end(), pmoves.begin(), pmoves.end());
            moves.insert(moves.end(), nmoves.begin(), nmoves.end());
            moves.insert(moves.end(), bmoves.begin(), bmoves.end());
            moves.insert(moves.end(), rmoves.begin(), rmoves.end());
            moves.insert(moves.end(), qmoves.begin(), qmoves.end());
            moves.insert(moves.end(), kmoves.begin(), kmoves.end());

            return moves;
        }
    }

    vector<vector<char>> knight_moves(long long board, long long same_col, long long diff_col) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                for (auto dir: DIR_N) {
                    if (!in_board(i%8+dir[1], i/8+8*dir[0])) continue;
                    char pos = i + 8*dir[0] + dir[1];
                    if (((1LL << pos) & same_col) == 0) moves.push_back({i, pos});
                }
            }
        }
        return moves;
    }

    vector<vector<char>> king_moves(long long board, long long same_col, long long diff_col, char castling) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                for (auto dir: DIR_K) {
                    if (!in_board(i%8+dir[1], i/8+8*dir[0])) continue;
                    char pos = i + 8*dir[0] + dir[1];
                    if (((1LL << pos) & same_col) == 0) moves.push_back({i, pos});
                }
            }
        }
        return moves;
    }

    vector<vector<char>> bishop_moves(long long board, long long same_col, long long diff_col) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                for (auto dir: DIR_B) {
                    for (auto dist = 1; dist < 8; dist++) {
                        if (!in_board(i%8+dir[1], i/8+8*dir[0])) break;
                        char pos = i + dist*8*dir[0] + dist*dir[1];
                        auto shift = 1LL << pos;
                        if ((shift & same_col) != 0) break;
                        moves.push_back({i, pos});
                        if ((shift & diff_col) != 0) break;
                    }
                }
            }
        }
        return moves;
    }
    
    vector<vector<char>> rook_moves(long long board, long long same_col, long long diff_col) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                for (auto dir: DIR_R) {
                    for (auto dist = 1; dist < 8; dist++) {
                        if (!in_board(i%8+dir[1], i/8+8*dir[0])) break;
                        char pos = i + dist*8*dir[0] + dist*dir[1];
                        auto shift = 1LL << pos;
                        if ((shift & same_col) != 0) break;
                        moves.push_back({i, pos});
                        if ((shift & diff_col) != 0) break;
                    }
                }
            }
        }
        return moves;
    }

    vector<vector<char>> queen_moves(long long board, long long same_col, long long diff_col) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                for (auto dir: DIR_B) {
                    for (auto dist = 1; dist < 8; dist++) {
                        if (!in_board(i%8+dir[1], i/8+8*dir[0])) break;
                        char pos = i + dist*8*dir[0] + dist*dir[1];
                        auto shift = 1LL << pos;
                        if ((shift & same_col) != 0) break;
                        moves.push_back({i, pos});
                        if ((shift & diff_col) != 0) break;
                    }
                }
            }
        }
        return moves;
    }

    vector<vector<char>> pawn_moves(long long board, long long same_col, long long diff_col, bool side, bool ep, char ep_square) {
        vector<vector<char>> moves;
        for (char i = 0; i < 64; i++) {
            if (bit(board, i)) {
                int speed = i/8 == (side ? 6 : 1) ? 2 : 1;
                int dir = side ? -1 : 1;
                for (auto dist = 1; dist < speed + 1; dist++) {
                    if (!in_board(i%8, i/8+8*dir)) break;
                    char pos = i + dist*8*dir;
                    auto shift = 1LL << pos;
                    if ((shift & same_col) != 0 || (shift & diff_col) != 0) break;
                    moves.push_back({i, pos});
                }
                char pos;
                pos = i + 8*dir - 1;
                if (in_board(i%8-1, i/8+8*dir) && (((1LL << pos) & diff_col) != 0)) moves.push_back({i, pos});
                pos = i + 8*dir + 1;
                if (in_board(i%8+1, i/8+8*dir) && (((1LL << pos) & diff_col) != 0)) moves.push_back({i, pos});
            }
        }
        return moves;
    }
}
