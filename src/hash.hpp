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

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "bitboard.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;

namespace Hash {
    constexpr unsigned short S2 = 41421;  // First 5 digits of sqrt(2) % 65536
    constexpr unsigned short S3 =  7769;  // First 5 digits of sqrt(3) % 65536
    constexpr unsigned short S5 = 23606;  // First 5 digits of sqrt(5) % 65536
    constexpr unsigned short S7 = 64575;  // First 5 digits of sqrt(7) % 65536
    const unsigned short RAND[256] = {    // Random numbers
        47864, 6350 , 6279 , 48555, 2498 , 29202, 11371, 11948, 23326, 36825, 58968, 8382 , 4326 , 54299, 10085, 4041 ,
        40881, 65152, 935  , 29158, 43852, 49576, 45416, 26000, 30483, 65345, 49381, 25663, 10867, 45031, 10010, 65061,
        47033, 36274, 31378, 40043, 57142, 3301 , 40356, 43167, 156  , 1269 , 53812, 43144, 35153, 63013, 20319, 16097,
        49371, 60416, 55808, 37381, 44047, 17620, 36248, 4739 , 40881, 25857, 47201, 17715, 4835 , 28677, 12492, 7081 ,
        20810, 13383, 57772, 9181 , 28153, 7833 , 9281 , 14178, 12548, 42989, 864  , 36330, 64428, 9861 , 6737 , 9102 ,
        43360, 57110, 50959, 29942, 41061, 5642 , 31033, 53840, 41687, 42953, 23905, 2868 , 3837 , 25584, 45699, 31979,
        63399, 43694, 28049, 57578, 39412, 46052, 7687 , 22982, 43760, 38802, 15517, 2373 , 30574, 7151 , 15236, 55696,
        13709, 54007, 31879, 62719, 61137, 27399, 59958, 21947, 9993 , 63352, 30039, 65436, 28005, 38999, 40423, 17394,
        19754, 19199, 56323, 29119, 29384, 49149, 32662, 2325 , 6030 , 49099, 13664, 44531, 30298, 62838, 20622, 62109,
        11155, 38600, 43291, 53118, 51914, 63692, 54537, 16877, 40209, 50852, 53225, 48371, 13253, 41024, 607  , 28009,
        16474, 46141, 8388 , 23244, 44732, 36737, 12023, 64761, 25109, 26802, 21204, 1394 , 20666, 34709, 1697 , 15969,
        39064, 29740, 58564, 18652, 56516, 2644 , 40885, 14584, 10006, 31566, 47277, 61814, 49987, 31824, 17411, 40975,
        46945, 20645, 41284, 60509, 8738 , 25740, 40844, 6625 , 24929, 51682, 47786, 22008, 64633, 7027 , 1690 , 7162 ,
        43544, 22657, 36326, 28369, 61382, 39576, 53690, 62408, 42363, 1311 , 40572, 40447, 8484 , 59418, 59996, 38570,
        22148, 45877, 20993, 4110 , 61877, 49363, 15241, 356  , 63057, 33758, 27273, 54581, 10555, 51264, 63936, 61541,
        21625, 32570, 15910, 7861 , 25671, 41134, 24877, 41181, 15126, 31055, 37897, 34692, 13608, 62497, 16989, 12345
    };

    U64 hash(const Position&);
}
