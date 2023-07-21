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
#include <fstream>
#include "hash.hpp"
#include "bitboard.hpp"
#include "options.hpp"
#include "utils.hpp"
#include "random.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::getline;


namespace Opening {
    vector<string> openings;

    void load_openings(Options& options) {
        ifstream openings_file(options.OwnBookPath);

        if (!openings_file.is_open()) {
            cerr << "Warning: could not open openings file." << endl;
        } else {
            while (openings_file) {
                string line;
                getline(openings_file, line);
                if (openings_file) {
                    openings.push_back(strip(line));
                }
            }
        }
    }

    vector<string> get_sequence(const string& moves) {
        vector<string> possible;

        for (string opening: openings) {
            if (startswith(opening, moves)) {
                if (opening.size() > moves.size()) {
                    possible.push_back(opening);
                }
            }
        }

        if (possible.empty()) {
            return {"", ""};
        }

        string sequence = strip(possible[Random::random()%possible.size()]);
        
        return {sequence, sequence.substr(moves.size(), 5)};
    }
}
