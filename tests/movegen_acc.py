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

#  Tests performance and accuracy in move generation.
#  This script will communicate "perft movegen <depth>" to the engine,
#  and the engine should output total nodes searched (not leaf nodes)
#  in the format "info nodes <nodes>"

import os
import sys
import time
import subprocess
import chess

PARDIR = os.path.dirname(os.path.realpath(__file__))
ENG_PATH = "build/Megalodon"
FEN = input("Testing FEN: ")


def real_result(pos: chess.Board):
    return [move.uci() for move in pos.generate_legal_moves()]


def engine_result():
    in_path = os.path.join(PARDIR, "in.txt")
    out_path = os.path.join(PARDIR, "out.txt")

    with open(in_path, "w") as file:
        file.write(f"position fen {FEN}\n")
        file.write(f"legalmoves\n")
    with open(in_path, "r") as stdin, open(out_path, "w") as stdout:
        p = subprocess.Popen([ENG_PATH], stdin=stdin, stdout=stdout)
        p.wait()
    with open(out_path, "r") as file:
        moves = file.read().split("\n")[1:]

    os.remove(in_path)
    os.remove(out_path)

    return moves


def main():
    start = time.time()
    real = real_result(chess.Board(FEN), DEPTH)
    elapse = time.time() - start
    engine, eng_time = engine_result()

    print(f"Python-Chess nodes:   {real}")
    print(f"Python-Chess time:    {elapse}")
    print(f"Python-Chess NPS:     {real/elapse}")
    print()
    print(f"Engine nodes:         {engine}")
    print(f"Engine time:          {eng_time}")
    print(f"Engine NPS:           {engine/eng_time}")


main()
