![logo](https://raw.githubusercontent.com/megalodon-chess/megalodon/main/logo/logo_widescreen_light.png)

# Megalodon

UCI chess engine

Play against Megalodon on [Lichess][lichess]!

[Version 1.0.0][latest] was recently released!

## Table of Contents

* [About][about]
* [Releases][releases]
* [Quick Start][quickstart]
* [Compiling][compiling]
* [ELO][elo]
* [Contributing][contrib]

## Version 1.0.0

Megalodon 1.0.0 release date has been pushed back to April 14th.

### Goals

* ELO: 1200
* Nodes per second: 30000
* Bitboard move generation
* Breadth first search
* Minimax move picking with alpha-beta pruning
* Prune lowest N% branches (bad branches), N defined by user option

### Checkpoints

* v0.1.0: Working move generation and DFS minimax.
* v0.1.x: Move generation optimization.
* v0.2.x: BFS move search with alpha-beta and bad branch pruning.
* v0.3.x: Evaluation improvements.
* v0.4.x: Final optimizations and prepare v1.0.0 release.

[lichess]: https://lichess.org/@/megalodon-chess
[latest]: https://github.com/megalodon-chess/megalodon/releases/latest
[about]: https://megalodon-chess.github.io/megalodon/about
[releases]: https://megalodon-chess.github.io/megalodon/releases
[quickstart]: https://megalodon-chess.github.io/megalodon/quick-start
[compiling]: https://megalodon-chess.github.io/megalodon/compiling
[elo]: https://megalodon-chess.github.io/megalodon/elo
[contrib]: https://github.com/megalodon-chess/megalodon/blob/main/CONTRIBUTING.md
