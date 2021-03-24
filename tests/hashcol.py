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

# Tries random positions until two with the same hash is found.

import os
import time
import subprocess
import random
import chess

PARENT = os.path.dirname(os.path.realpath(__file__))
ENG_PATH = "build/Megalodon"


def main():
    hashes = []
    while True:
        board = chess.Board()
        num_moves = random.randint(10, 60)
        for i in range(num_moves):
            moves = list(board.generate_legal_moves())
            if len(moves) == 0:
                break
            board.push(random.choice(moves))

        p = subprocess.Popen([ENG_PATH], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        p.stdin.write(f"position startpos moves ".encode())
        for move in board.move_stack:
            p.stdin.write(f"{move.uci()} ".encode())
        p.stdin.write(b"\n")
        p.stdin.write(f"hash\n".encode())
        p.stdin.write(f"quit\n".encode())
        p.stdin.flush()
        while p.poll() is None:
            time.sleep(0.01)
        out = b""
        while len(d := p.stdout.read(1)) > 0:
            out += d
        out = out.decode().split("\n")

        for line in out:
            if line.isnumeric():
                h = int(line)
                break

        if h in hashes:
            print(h)
            print("position startpos ", end="")
            for m in board.move_stack:
                print(m.uci(), end=" ")
            print()
            print(f"Tried {len(hashes)} positions.")
            break
        hashes.append(h)


main()
