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
DEPTH = 4
FEN = chess.STARTING_FEN

if len(sys.argv) > 1:
    if sys.argv[1].isnumeric():
        DEPTH = int(sys.argv[1])
    else:
        print(f"Invalid depth: {sys.argv[1]}")

print(f"Using depth {DEPTH}")

def real_result(pos: chess.Board, depth):
    if depth == 0:
        return 1
    else:
        count = 1
        for move in pos.generate_legal_moves():
            new_pos = pos.copy(stack=False)
            new_pos.push(move)
            count += real_result(new_pos, depth-1)
        return count


def engine_result():
    p = subprocess.Popen([ENG_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
    p.stdin.write(f"position fen {FEN}\n".encode())
    p.stdin.write(f"go perft {DEPTH}\n".encode())
    p.stdin.write(f"quit\n".encode())
    p.stdin.flush()
    while p.poll() is None:
        time.sleep(0.01)
    out = b""
    while len(d := p.stdout.read(1)) > 0:
        out += d
    out = out.decode()

    parts = [l for l in out.split("\n") if l.startswith("info")][-1].split()
    for i in range(len(parts)):
        if parts[i] == "nodes":
            nodes = int(parts[i+1])
        elif parts[i] == "time":
            elapse = int(parts[i+1]) / 1000

    return (nodes, elapse)


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
