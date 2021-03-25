# Releases

This page contains release info.

## [Megalodon 0.2.4][v0.2.4]

This release features a hash table and searches a principal variation.

The hash algorithm currently has about 1 collision every 10000 hashes, so it is extremely unreliable.
We recommend not using the hash table in this version.

To toggle hash table on and off:

```
setoption name UseHashTable value true
setoption name UseHashTable value false
```

## [Megalodon 0.2.3][v0.2.3]

This release features improved search and move generation.

The evaluation has been reduced to material only, and it will be developed in v0.3.x

The move generation reaches 900k nodes per second, a 20% improvement from the previous version.

Iterative Deepening has been implemented into the new search algorithm, but there are no
improvements over single depth alpha-beta yet.

## [Megalodon 0.2.2][v0.2.2]

This release contains evaluation improvements and legal move bug fixes.

## [Megalodon 0.2.1][v0.2.1]

This release contains many bug fixes and evaluation improvements.

Now, Megalodon plays developing moves in the opening, and the developers estimate it plays at 900 ELO.

The NPS has dropped to 27000, but this will be improved in v0.3.x

## [Megalodon 0.2.0][v0.2.0]

This release features alpha-beta pruning minimax, and new evaluation functions.

In this release, the engine evaluates material, king, and pawns.
Although currently it can only play as white, it plays at an estimated 800 ELO.

In the opening, the engine plays random moves, but in the middle game,
it can see simple tactics.

## [Megalodon 0.1.3][v0.1.3]

There are many move generation bug fixes in this release,
and the engine has almost perfect move generation.

It now also evaluates mate, but there are still some bugs.

## [Megalodon 0.1.2][v0.1.2]

Bit processing instructions were greatly optimized in this release.

The NPS increased from 200k to 240K.

This release also includes the Depth First Search algorithm used in versions preceding v0.1.0.
You can choose what search algorithm (BFS or DFS) to use.
The engine defaults to BFS, which is much faster.

## [Megalodon 0.1.1][v0.1.1]

This version features a new search algorithm, which is non-recursive and Breadth First.

It searches at an amazing 200,000 NPS, with a hard-coded depth of 5.

The move generation algorithm was also improved, but fails on some positions.
If Megalodon thinks for a long time (more than 45 seconds) without moving, it is likely that the
program has crashed.

## [Megalodon 0.1.0][v0.1.0]

A few hours of working after v0.1.0-alpha, v0.1.0 is out!

The move generation algorithm is almost fully working, and sometimes misses en passant moves.

It can generate moves at 550,000 NPS (without minimax or evaluation),
and the final playing NPS is about 35,000.

The engine is set to run DFS search at depth 4 each move. It thinks for 5-15 seconds.

The next releases (v0.1.x) will be focused on developing the search algorithm.

## [Megalodon 0.1.0 Alpha][v0.1.0-alpha]

This is a pre-release to Megalodon 0.1.0, which will feature fully working bitboards!

This version does not function fully, and will likely generate illegal moves.

## [Megalodon 0.0.3][v0.0.3]

This release features improved board algorithms, which increase it's NPS (nodes per second) from 4000 to 8000.

The depth of calculation was also adjusted to 4, and Megalodon will think for about 30 seconds per move.

After this release, the Megalodon team will work on implementing bitboards.

## [Megalodon 0.0.2][v0.0.2]

This release includes updated searching algorithms and bug fixes.

## [Megalodon 0.0.1][v0.0.1]

This is the first release of Megalodon, a UCI chess engine! It does not function fully yet.

Megalodon uses a minimax search with material and piece maps as evaluation.

[Back to documentation home][home]

[home]: https://megalodon-chess.github.io/megalodon/
[v0.2.4]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.4
[v0.2.3]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.3
[v0.2.2]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.2
[v0.2.1]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.1
[v0.2.0]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.0
[v0.1.3]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.1.3
[v0.1.2]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.1.2
[v0.1.1]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.1.1
[v0.1.0]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.1.0
[v0.1.0-alpha]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.1.0-alpha
[v0.0.3]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.0.3
[v0.0.2]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.0.2
[v0.0.1]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.0.1
