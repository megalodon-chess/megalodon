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

FILES = chess/funcs.o chess/constants.o chess/board.o chess/move.o eval.o search.o uci.o main.o
CFILES = chess/funcs.cpp chess/constants.cpp chess/board.cpp chess/move.cpp eval.cpp search.cpp uci.cpp main.cpp
.PHONY: all dev compile clean run

# Compile and clean files.
all: compile clean

# Compile, clean, and run executable.
dev: compile clean run

# Uses g++ to compile files.
compile: $(FILES)
	g++ -std=c++17 -o Megalodon $(FILES)

# Compiles for Windows. Doesn't work fully.
compilewin: $(FILES)
	x86_64-w64-mingw32-g++-win32 -o Megalodon-Windows $(CFILES)

# Removes all c++ objects and clears console.
clean:
	rm *.o
	rm chess/*.o
	clear

# Runs executable.
run:
	./Megalodon
