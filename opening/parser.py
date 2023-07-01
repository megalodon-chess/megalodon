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

import pandas as pd
import chess
import chess.pgn
from io import StringIO

# https://github.com/Destaq/chess-graph/blob/master/elo_reading/openings_sheet.csv
OPENINGS_FILE = "openings.csv"

data = pd.read_csv(OPENINGS_FILE)
openings = set()

for row in range(len(data)):
    move_string = data.iloc[row]["moves"]
    moves = move_string.split()

    groups_of_2 = [" ".join(moves[i:i+2]) for i in range(0, len(moves), 2)]
    number_list = [f"{i}." for i in range(1, len(groups_of_2) + 1)]
    pgn = " ".join([" ".join(pair) for pair in zip(number_list, groups_of_2)])

    game = chess.pgn.read_game(StringIO(pgn))
    board = chess.Board()
    for move in game.mainline_moves():
        board.push(move)
    uci_moves = ' '.join(map(str, board.move_stack))
    
    openings.add(uci_moves)


pd.DataFrame(sorted(list(openings))).to_csv("parsed_openings.txt", index=False)
