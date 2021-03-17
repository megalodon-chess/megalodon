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
from termcolor import cprint
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
        out = file.read().split("\n")
        for i, text in enumerate(out):
            if text.isdigit():
                moves = out[i+1:-1]
                break
        else:
            moves = []

    os.remove(in_path)
    os.remove(out_path)

    return moves


def main():
    engine = engine_result()
    len_engine = len(engine)
    real = real_result(chess.Board(FEN))
    len_real = len(real)

    print(f"Number of engine moves: ", end="")
    cprint(len_engine, "green" if len_engine == len_real else "red")
    print(f"Number of real moves:   {len_real}")

    if len_real > len_engine:
        for move in engine:
            print(move, end=" ")
            cprint(move, "green" if move in real else "red")
            real.remove(move)

        print("Moves not found: ", end="")
        cprint(", ".join(real), "red")

    elif len_real < len_engine:
        for move in real:
            print(move, end=" ")
            cprint(move, "green" if move in engine else "red")
            engine.remove(move)
        
        print("Extra moves found: ", end="")
        cprint(", ".join(engine), "red")

    else:
        for move in real:
            print(move, end=" ")
            cprint(move, "green")
        
        cprint("All moves successfully found!", "green")


main()
