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
    const string OPENINGS_FILE = "openings.txt";
    vector<string> openings;

    void init() {
        ifstream openings_file(OPENINGS_FILE);

        if (!openings_file.is_open()) {
            cerr << "Warning: could not open openings file." << endl;
        } else {
            while (openings_file) {
                string line;
                getline(openings_file, line);
                openings.push_back(line);
            }
        }
    }

    string get_move(const string& moves) {
        vector<string> possible;

        for (string opening: openings) {
            if (startswith(opening, moves)) {
                possible.push_back(opening.substr(moves.size(), 4));
            } 
        }

        return possible[Random::random()%possible.size()];
    }
}
