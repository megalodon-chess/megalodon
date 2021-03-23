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
    p = subprocess.Popen([ENG_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    p.stdin.write(f"position fen {FEN}\n".encode())
    p.stdin.write(f"legalmoves\n".encode())
    p.stdin.write(f"quit\n".encode())
    p.stdin.flush()
    while p.poll() is None:
        time.sleep(0.01)
    out = b""
    while len(d := p.stdout.read(1)) > 0:
        out += d
    out = out.decode().split("\n")

    for i, text in enumerate(out):
        if text.isdigit():
            moves = out[i+1:-1]
            break
    else:
        moves = []

    return moves


def main():
    engine = engine_result()
    len_engine = len(engine)
    real = real_result(chess.Board(FEN))
    len_real = len(real)

    print(f"Number of engine moves: ", end="")
    cprint(len_engine, "green" if len_engine == len_real else "red")
    print(f"Number of real moves:   {len_real}")

    correct = [x for x in engine if x in real]
    if len_real > len_engine:
        for move in correct:
            print(move, end=" ")
            cprint(move, "green")
            real.remove(move)
            engine.remove(move)

        for i in range(len(engine)):
            print(real[i], end=" ")
            cprint(engine[i], "red")
            real.pop(i)

        print("Moves not found: ", end="")
        cprint(", ".join(real), "red")

    elif len_real < len_engine:
        for move in correct:
            print(move, end=" ")
            cprint(move, "green")
            real.remove(move)
            engine.remove(move)

        for i in range(len(real)):
            print(real[i], end=" ")
            cprint(engine[i], "red")
            engine.pop(i)

        print("Extra moves found: ", end="")
        cprint(", ".join(engine), "red")

    else:
        for move in correct:
            print(move, end=" ")
            cprint(move, "green")
            real.remove(move)
            engine.remove(move)

        for i in range(len(real)):
            print(real[i], end=" ")
            cprint(engine[i], "red")

        errors = len_engine - len(correct)

        if errors:
            cprint(f"{errors} errors were found...", "red")
        else:
            cprint("No errors found!", "green")


main()
