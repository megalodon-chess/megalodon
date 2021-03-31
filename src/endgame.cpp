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
#include "bitboard.hpp"
#include "endgame.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


namespace Endgame {
    bool cnt_match(const vector<char> c1, const vector<char> c2) {
        for (char i = 0; i < 10; i++) {
            if (c1[i] != c2[i]) return false;
        }
        return true;
    }

    vector<char> get_cnts(const Position& pos) {
        const char wpc = Bitboard::popcnt(pos.wp);
        const char wnc = Bitboard::popcnt(pos.wn);
        const char wbc = Bitboard::popcnt(pos.wb);
        const char wrc = Bitboard::popcnt(pos.wr);
        const char wqc = Bitboard::popcnt(pos.wq);
        const char bpc = Bitboard::popcnt(pos.bp);
        const char bnc = Bitboard::popcnt(pos.bn);
        const char bbc = Bitboard::popcnt(pos.bb);
        const char brc = Bitboard::popcnt(pos.br);
        const char bqc = Bitboard::popcnt(pos.bq);
        return {wpc, wnc, wbc, wrc, wqc, bpc, bnc, bbc, brc, bqc};
    }

    int eg_type(const Position& pos) {
        const vector<char> counts = get_cnts(pos);

        if (pos.turn && cnt_match(W_KQvK, counts)) {
            return 1;
        } else if (!pos.turn && cnt_match(B_KQvK, counts)) {
            return 1;
        }

        return 0;
    }


    Move bestmove(const Position& pos, const vector<Move>& moves, const int& eg) {
        const vector<char> counts = get_cnts(pos);
        if (pos.turn) {
            switch (eg) {
                case 1: return kqvk(moves, pos.wk, pos.wq, pos.bk);
            }
        } else {
            switch (eg) {
                case 1: return kqvk(moves, pos.bk, pos.bq, pos.wk);
            }
        }
    }


    Move kqvk(const vector<Move>& moves, const U64& _ck, const U64& _cq, const U64& _ok) {
        const Location ck = Bitboard::first_bit(_ck);
        const Location cq = Bitboard::first_bit(_cq);
        const Location ok = Bitboard::first_bit(_ok);
        const char ckp = (ck.y<<3) + ck.x;
        const char cqp = (cq.y<<3) + cq.x;
        const char okp = (ok.y<<3) + ok.x;

        // TODO avoid stalemate.
        char best_dist = 16;
        Move best_move = moves[0];
        for (const auto& move: moves) {
            if (move.from == cqp) {
                const Location to = Location(move.to);
                const char dx = abs(to.x-ok.x);
                const char dy = abs(to.y-ok.y);
                if ((dx==0 || dx==1) && (dy==0 || dy==1)) continue;  // If move goes to opponent's king
                if (dx == 0 || dy == 0) continue;  // Don't check king

                const char dist = dx + dy;
                if (dist < best_dist) {
                    best_move = move;
                    best_dist = dist;
                }
            }
        }

        return best_move;
    }
}
