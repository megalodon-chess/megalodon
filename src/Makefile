#
#  Megalodon
#  UCI chess engine
#  Copyright Patrick Huang and Arjun Sahlot 2021
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

.PHONY: main clean

all: main clean

main: chess/funcs.o chess/constants.o chess/board.o chess/move.o eval.o main.o
	g++ chess/funcs.o chess/constants.o chess/board.o chess/move.o eval.o main.o -o Megalodon

clean:
	rm *.o
	rm chess/*.o
