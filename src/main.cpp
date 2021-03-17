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
#include "utils.hpp"
#include "bitboard.hpp"
#include "uci.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


void print_info() {
    cout << "Megalodon v0.2.0 - UCI chess engine" << "\n";
    cout << "Copyright the Megalodon developers (in AUTHORS file)" << "\n";
    cout << "https://github.com/megalodon-chess/megalodon" << "\n";
    cout << "Licensed under GNU GPL v3: https://www.gnu.org/licenses/" << endl;
}


int main() {
    srand(time(NULL));
    print_info();
    return loop();
}
