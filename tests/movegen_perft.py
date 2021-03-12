#
#  Megalodon
#  UCI chess engine
#  Copyright the Megalodon developers
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
# and the engine should output total nodes searched (not leaf nodes)
# in the format "info nodes <nodes>"

import os
import time
import subprocess
import chess

PARDIR = os.path.dirname(os.path.realpath(__file__))
ENG_PATH = "build/Megalodon"
DEPTH = 4


def engine_result():
    in_path = os.path.join(PARDIR, "in.txt")
    out_path = os.path.join(PARDIR, "out.txt")

    with open(in_path, "w") as file:
        file.write(f"go depth {DEPTH}\n")
    with open(in_path, "r") as stdin, open(out_path, "w") as stdout:
        start = time.time()
        p = subprocess.Popen([ENG_PATH], stdin=stdin, stdout=stdout)
        p.wait()
        elapse = time.time() - start
    with open(out_path, "r") as file:
        out = file.read()

    parts = [l for l in out.split("\n") if l.startswith("info")][0].split()
    for i in range(len(parts)):
        if parts[i] == "nodes":
            nodes = int(parts[i+1])
            break

    return (nodes, elapse)


def main():
    engine, eng_time = engine_result()


main()