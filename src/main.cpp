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
#include "uci.hpp"
#include "bitboard.hpp"
#include "hash.hpp"
#include "eval.hpp"
#include "search.hpp"
#include "utils.hpp"

#define VERSION  "1.0.0"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


void print_info() {
    cout << "Megalodon v" << VERSION << " - UCI chess engine" << "\n";
    cout << "Build date: " << __DATE__ << ", " << __TIME__ << endl;
    cout << "Copyright the Megalodon developers (in AUTHORS file)" << "\n";
    cout << "https://github.com/megalodon-chess/megalodon" << "\n";
    cout << "Licensed under GNU GPL v3: https://www.gnu.org/licenses/" << "\n";
}


int main(const int argc, const char* argv[]) {
    cout << std::fixed;
    srand(1234);

    Hash::init();
    Eval::init();

    if (argc >= 2) {
        if (argv[1] == string("--version")) cout << VERSION << endl;
    } else {
        print_info();
        return loop();
    }
}
