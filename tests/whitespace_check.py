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

# Checks if there is trailing whitespace in source files.

import os

PARENT = os.path.dirname(os.path.realpath(__file__))
FILE_DIR = os.path.join(os.path.dirname(PARENT), "src")


def main():
    exitcode = 0

    for f in os.listdir(FILE_DIR):
        path = os.path.join(FILE_DIR, f)
        relpath = os.path.join("src", f)
        with open(path, "r") as file:
            lines = file.read().split("\n")
            for i, l in enumerate(lines):
                if l.endswith(" "):
                    print(f"Trailing whitespace in file {relpath}, line {i+1}")
                    exitcode = 1

    return exitcode


exit(main())
