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

# Tests movegen accuracy on random positions.

import os
import subprocess
import random
import chess

PARENT = os.path.dirname(os.path.realpath(__file__))
IN = os.path.join(PARENT, "in.txt")
OUT = os.path.join(PARENT, "out.txt")
PATH = os.path.join(PARENT, "Megalodon")


def main():
    while True:
        board = chess.Board()
        num_moves = random.randint(10, 60)
        for i in range(num_moves):
            moves = list(board.generate_legal_moves())
            if len(moves) == 0:
                break
            board.push(random.choice(moves))

        with open(IN, "w") as file:
            file.write("position startpos moves ")
            for move in board.move_stack:
                file.write(move.uci() + " ")
            file.write("\n")
            file.write("legalmoves")

        with open(IN, "r") as stdin, open(OUT, "w") as stdout:
            subprocess.Popen([PATH], stdin=stdin, stdout=stdout).wait()

        with open(OUT, "r") as file:
            data = file.read().split("\n")
        for i, line in enumerate(data):
            if line.isnumeric():
                count = int(line)
                eng_moves = data[i:i+count]
                break

        if set(eng_moves) != set([m.uci() for m in board.move_stack]):
            print("position startpos moves ", end="")
            for m in board.move_stack:
                print(m.uci(), end=" ")
            print()
            break

    os.remove(IN)
    os.remove(OUT)


main()
