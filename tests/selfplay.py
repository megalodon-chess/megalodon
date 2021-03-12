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

# Makes Megalodon play against itself repeatedly.

import os
import chess
import chess.engine
import chess.pgn

PARDIR = os.path.dirname(os.path.realpath(__file__))
GAME_DIR = os.path.join(PARDIR, "games")


def save_game(board: chess.Board):
    if len(os.listdir(GAME_DIR)) > 0:
        num = max([int(f.split(".")[0]) for f in os.listdir(GAME_DIR)]) + 1
    else:
        num = 0
    fname = os.path.join(GAME_DIR, f"{num}.pgn")

    game = chess.pgn.Game()
    game.headers["Event"] = "Megalodon self-play"
    game.headers["Round"] = str(num)
    game.headers["White"] = "Megalodon"
    game.headers["Black"] = "Megalodon"
    node = game.add_variation(board.move_stack[0])
    for move in board.move_stack[1:]:
        node = node.add_variation(move)

    with open(fname, "w") as file:
        print(game, file=file)


def main():
    eng1 = chess.engine.SimpleEngine.popen_uci("build/Megalodon")
    eng2 = chess.engine.SimpleEngine.popen_uci("build/Megalodon")

    while True:
        board = chess.Board()
        while not board.is_game_over():
            result = eng1.play(board, chess.engine.Limit(white_clock=10, black_clock=10))
            board.push(result.move)
            result = eng2.play(board, chess.engine.Limit(white_clock=10, black_clock=10))
            board.push(result.move)

        save_game(board)


main()
