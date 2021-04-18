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

# Downloads and solves https://lichess.org daily puzzle.

import requests
import termcolor
import chess
import chess.pgn
import chess.engine

PATH = "./build/Megalodon"
ENDPOINT = "https://lichess.org/api/puzzle/daily"
LIMIT = chess.engine.Limit(time=25)   # Adjust as needed to get correct solution


def get_puzzle():
    print("Getting puzzle...", flush=True, end="")

    data = requests.get(ENDPOINT).json()
    board = chess.Board()
    for move in data["game"]["pgn"].split():
        board.push_san(move)

    solution = [chess.Move.from_uci(uci) for uci in data["puzzle"]["solution"]]

    print("Done", flush=True)
    return (board, solution)


def solve_puzzle(board, solution):
    engine = chess.engine.SimpleEngine.popen_uci(PATH)
    print(f"Solving puzzle ({len(solution)} moves)")
    print(f"Starting position is {board.fen()}")

    for i, move in enumerate(solution):
        if i % 2 == 1:  # Only solve first side moves.
            board.push(move)
            continue

        print(termcolor.colored("", "white"), end="")
        print(f"Move {i+1}...", end="", flush=True)
        result = engine.play(board, LIMIT, info=chess.engine.INFO_ALL)
        if result.move == move:
            board.push(move)
            print(termcolor.colored("Correct! Move is {}, eval is {}, depth {}, {} nodes".format(move.uci(),
                result.info["score"].relative, result.info["depth"], result.info["nodes"]), "green"))
        else:
            print(termcolor.colored("Wrong. Answer is {}, engine played {}".format(move.uci(), result.move.uci()), "red"))
            break

    print(termcolor.colored("", "white"), end="")
    engine.quit()


def main():
    board, solution = get_puzzle()
    solve_puzzle(board, solution)


main()
