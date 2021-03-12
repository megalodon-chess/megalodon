#
#  Megalodon
#  UCI chess engine
#  Copyright Patrick Huang and Arjun Sahlot 2021
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <https://www.gnu.org/licenses/>.
#

# Tests performance and accuracy in move generation.
# This script will communicate "go depth <depth>" to the engine,
# and the engine should output total nodes searched (not leaf nodes).

import os
import subprocess
import chess

PARDIR = os.path.dirname(os.path.realpath(__file__))
ENG_PATH = "build/Megalodon"
DEPTH = 4


def main():
    with open(os.path.join(PARDIR, "in.txt", "w")) as file:
        file.write(f"go depth {DEPTH}\n")

    with open(os.path.join(PARDIR, "in.txt"), "r") as stdin, open(os.path.join(PARDIR, "out.txt"), "w") as stdout:
        p = subprocess.Popen([ENG_PATH], stdin=stdin, stdout=stdout)
        p.wait()


main()
