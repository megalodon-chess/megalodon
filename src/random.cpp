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

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;


namespace Random {
    unsigned long long curr = get_time();

    void set_seed(unsigned long long s) {
        curr = s;
        if (curr == 0) curr = 1;
    }

    unsigned long long random() {
        curr *= 16807;
        if (curr == 0) curr = 1;
        return curr;
    }
}
