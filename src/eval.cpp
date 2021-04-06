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
#include "options.hpp"
#include "eval.hpp"
#include "search.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

using Bitboard::popcnt;
using Bitboard::bit;


namespace Eval {
    char CENTER_DIST_MAP[64];

    void init() {
        for (auto i = 0; i < 64; i++) CENTER_DIST_MAP[i] = center_dist(i);
    }


    float material(const Position& pos) {
        float value = 0;
        value += popcnt(pos.wp) * 1;
        value += popcnt(pos.wn) * 3;
        value += popcnt(pos.wb) * 3;
        value += popcnt(pos.wr) * 5;
        value += popcnt(pos.wq) * 9;
        value -= popcnt(pos.bp) * 1;
        value -= popcnt(pos.bn) * 3;
        value -= popcnt(pos.bb) * 3;
        value -= popcnt(pos.br) * 5;
        value -= popcnt(pos.bq) * 9;
        return value;
    }

    float total_mat(const Position& pos) {
        float value = 0;
        value += popcnt(pos.wp) * 1;
        value += popcnt(pos.wn) * 3;
        value += popcnt(pos.wb) * 3;
        value += popcnt(pos.wr) * 5;
        value += popcnt(pos.wq) * 9;
        value += popcnt(pos.bp) * 1;
        value += popcnt(pos.bn) * 3;
        value += popcnt(pos.bb) * 3;
        value += popcnt(pos.br) * 5;
        value += popcnt(pos.bq) * 9;
        return value;
    }

    float non_pawn_mat(const Position& pos) {
        float value = 0;
        value += popcnt(pos.wn) * 3;
        value += popcnt(pos.wb) * 3;
        value += popcnt(pos.wr) * 5;
        value += popcnt(pos.wq) * 9;
        value += popcnt(pos.bn) * 3;
        value += popcnt(pos.bb) * 3;
        value += popcnt(pos.br) * 5;
        value += popcnt(pos.bq) * 9;
        return value;
    }


    float phase(const Position& pos) {
        // 1 = full middlegame, 0 = full endgame.
        const float npm = non_pawn_mat(pos);
        if (npm >= MIDGAME_LIM) return 1;
        else if (npm <= ENDGAME_LIM) return 0;
        else return ((float)(npm-ENDGAME_LIM) / (MIDGAME_LIM-ENDGAME_LIM));
    }

    float middle_game(const float& pawn_struct, const float& knight, const float& king, const float& space) {
        return (
            pawn_struct *  0.9F +
            knight      *  0.2F +
            king        *  0.2F +
            space       *  1.F
        );
    }

    float end_game(const float& pawn_struct, const float& knight, const float& king, const float& space) {
        return (
            pawn_struct *  1.2F +
            knight      *  0.1F +
            king        * -0.3F +
            space       *  0.F        // Space encourages pawns in the center, which discourages promotion.
        );
    }


    char center_dist(const char& i) {
        const char x = i&7, y = i>>3;
        const char xdist = (x<=3) ? 3-x : x-4;
        const char ydist = (y<=3) ? 3-y : y-4;
        return xdist + ydist;
    }


    float pawn_structure(const U64& wp, const U64& bp) {
        // Values represent white - black
        char stacked = 0;
        char islands = 0;
        char passed = 0;
        char backwards = 0;

        // Generate file values
        char w_adv[8], b_adv[8];     // Position of most advanced pawn, side dependent (-1 if no pawn)
        char w_back[8], b_back[8];   // Position of least advanced pawn, side dependent (-1 if no pawn)
        U64 w_files[8], b_files[8];  // Bitboards of pawns on each file
        for (char i = 0; i < 8; i++) {
            w_files[i] = wp & Bitboard::FILES[i];
            b_files[i] = bp & Bitboard::FILES[i];

            const U64 w = w_files[i] >> i;
            if (w == 0) {
                w_adv[i] = -1;
                w_back[i] = -1;
            } else {
                bool found = false;
                for (char j = 7; j > -1; j--) {
                    if ((w & (1ULL<<(j*8))) != 0) {
                        if (!found) w_adv[i] = j;
                        w_back[i] = j;
                        found = true;
                    }
                }
            }

            const U64 b = b_files[i] >> i;
            if (b == 0) {
                b_adv[i] = -1;
                b_back[i] = -1;
            } else {
                bool found = false;
                for (char j = 7; j > -1; j--) {
                    if ((b & (1ULL<<(j*8))) != 0) {
                        if (!found) b_adv[i] = j;
                        b_back[i] = j;
                        found = true;
                    }
                }
            }
        }

        // Stacked and islands
        bool w_on = false;
        bool b_on = false;
        for (char i = 0; i < 8; i++) {
            if (w_files[i] == 0) w_on = false;
            else {
                if (!w_on) islands++;
                w_on = true;
            }
            if (b_files[i] == 0) b_on = false;
            else {
                if (!b_on) islands--;
                b_on = true;
            }

            const char wcnt = popcnt(w_files[i]);
            const char bcnt = popcnt(b_files[i]);
            if (wcnt >= 2) stacked += (wcnt-1);
            if (bcnt >= 2) stacked -= (bcnt-1);
        }

        // Passed
        for (char i = 0; i < 8; i++) {
            if (w_files[i] != 0) {
                const bool blocked_right = (i>=1) && (b_back[i-1]!=-1) && (w_adv[i]>=b_back[i-1]);
                const bool blocked_left = (i<=6) && (b_back[i+1]!=-1) && (w_adv[i]>=b_back[i+1]);
                if (!blocked_right && !blocked_left) passed++;
            }
            if (b_files[i] != 0) {
                const bool blocked_right = (i>=1) && (w_back[i-1]!=-1) && (b_adv[i]<=w_back[i-1]);
                const bool blocked_left = (i<=6) && (w_back[i+1]!=-1) && (b_adv[i]<=w_back[i+1]);
                if (!blocked_right && !blocked_left) passed--;
            }
        }

        return (
            -0.3 * islands +
            -0.2 * stacked +
            0.6 * passed
        );
    }

    float space(const U64& wp, const U64& bp) {
        float sp = 0;

        for (char x = 2; x < 6; x++) {
            for (char y = 1; y < 5; y++) if (bit(wp, (y<<3)+x)) sp += y-1;
            for (char y = 3; y < 7; y++) if (bit(bp, (y<<3)+x)) sp -= 6-y;
        }

        return sp / 6;
    }

    float knights(const U64& wn, const U64& bn, const U64& wp, const U64& bp) {
        float wdist = 0, bdist = 0;
        const char wcnt = popcnt(wn), bcnt = popcnt(bn);
        const bool wp_in_cent = ((INNER_CENTER|OUTER_CENTER) & wp) != 0;
        const bool bp_in_cent = ((INNER_CENTER|OUTER_CENTER) & bp) != 0;

        for (char i = 0; i < 64; i++) {
            if (wp_in_cent && bit(wn, i)) {
                wdist += 6 - CENTER_DIST_MAP[i];
            } else if (bp_in_cent && bit(bn, i)) {
                bdist += 6 - CENTER_DIST_MAP[i];
            }
        }

        if (wcnt > 0) wdist /= wcnt;
        if (bcnt > 0) bdist /= bcnt;
        return wdist - bdist;
    }

    float kings(const U64& wk, const U64& bk) {
        const Location w = Bitboard::first_bit(wk);
        const Location b = Bitboard::first_bit(bk);
        const char wdist = CENTER_DIST_MAP[(w.y<<3)+w.x];
        const char bdist = CENTER_DIST_MAP[(b.y<<3)+b.x];
        return wdist - bdist;
    }


    float eval(const Options& options, const Position& pos, const vector<Move>& moves, const int& depth, const U64& o_attacks) {
        if (moves.empty()) {
            bool checked;
            if (pos.turn && ((o_attacks & pos.wk) != 0)) checked = true;
            else if (!pos.turn && ((o_attacks & pos.bk) != 0)) checked = true;
            if (checked) {
                // Increment value by depth to encourage sooner mate.
                // The smaller depth is, the closer it is to the leaf nodes.
                if (pos.turn) return Search::MIN - depth;  // Mate by black
                else return Search::MAX + depth;           // Mate by white
            }
            return 0;
        }

        const float mat         = material(pos);
        const float pawn_struct = options.EvalPawnStruct * pawn_structure(pos.wp, pos.bp);
        const float knight      = options.EvalKnights    * knights(pos.wn, pos.bn, pos.wp, pos.bp);
        const float king        = options.EvalKings      * kings(pos.wk, pos.bk);
        const float sp          = options.EvalSpace      * space(pos.wp, pos.bp);

        // Endgame and middle game are for weighting categories.
        const float mg = middle_game(pawn_struct, knight, king, sp);
        const float eg = end_game(pawn_struct, knight, king, sp);
        const float p = phase(pos);
        const float imbalance = mg*p + eg*(1-p);

        return mat + 0.4*imbalance;
    }
}
