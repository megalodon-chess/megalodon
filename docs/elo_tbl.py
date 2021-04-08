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

import os
import pygame
import json
import colorsys
pygame.init()

PARENT = os.path.dirname(os.path.realpath(__file__))
ELO_PATH = os.path.join(PARENT, "elo.json")
OUT = os.path.join(PARENT, "elo.png")

WIDTH, HEIGHT = 120, 60


def main():
    # Initialize variables
    with open(ELO_PATH, "r") as file:
        results = json.load(file)

    image = pygame.Surface((WIDTH * (len(results)+1), HEIGHT * (len(results)+1)))
    image.fill((255, 255, 255))
    font = pygame.font.SysFont("ubuntu", 24)

    # Find difference bounds
    min_diff = float("inf")
    max_diff = float("-inf")
    for x in range(len(results)):
        for y in range(len(results)):
            if x != y:
                eng1, score1 = results[x]
                eng2, score2 = results[y]
                diff = score1 - score2
                max_diff = max(diff, max_diff)
                min_diff = min(diff, min_diff)

    # Draw squares
    for x in range(len(results)):
        for y in range(len(results)):
            if x == y:
                color = (255, 255, 255)
                text = ""
            else:
                eng1, score1 = results[x]
                eng2, score2 = results[y]
                diff = int(10*(score1-score2)) / 10
                fac = (diff-min_diff) / (max_diff-min_diff)
                color = [x*255 for x in colorsys.hsv_to_rgb(fac/3, 0.8, 1)]
                text = str(diff)
                if diff >= 0:
                    text = "+" + text
                if "." not in text:
                    text += ".0"

            x_loc, y_loc = WIDTH * (x+1), HEIGHT * (y+1)
            pygame.draw.rect(image, color, (x_loc, y_loc, WIDTH, HEIGHT))
            t = font.render(text, 1, (0, 0, 0))
            t_loc = (x_loc + 20, y_loc + (HEIGHT-t.get_height())//2)
            image.blit(t, t_loc)

    # Draw headers
    for i in range(len(results)):
        eng, score = results[i]
        text = font.render(eng, 1, (0, 0, 0))
        offset = (HEIGHT-t.get_height())//2
        x = WIDTH*(i+1) + 20
        y = HEIGHT*(i+1) + offset
        image.blit(text, (x, offset))
        image.blit(text, (20, y))

    # Draw grid
    for i in range(len(results)):
        x = WIDTH * (i+1)
        y = HEIGHT * (i+1)
        pygame.draw.line(image, (0, 0, 0), (x, 0), (x, (len(results)+1)*HEIGHT))
        pygame.draw.line(image, (0, 0, 0), (0, y), ((len(results)+1)*WIDTH, y))

    pygame.image.save(image, OUT)


main()
