# Releases

This page contains release info.

## [Megalodon 0.4.2][v0.4.2]

This release features improved evaluation functions.

New evaluation categories include queens, rooks, pawn attacks, and weights.

## [Megalodon 0.4.1][v0.4.1]

This release contains many bug fixes.
The move generation is perfect when compared to [these results](https://www.chessprogramming.org/Perft_Results).

50 move draw has been implemented.
There are many projects going on about search, multithreading, eval, and Monte Carlo Tree Search.
Those may be merged shortly.

Bench: 2196682

## [Megalodon 0.4.0][v0.4.0]

This release contains many bug fixes in move generation.
Thanks to Guenther Simon (@rwbc) for reporting bugs.

The speed of everything was increased by about 5 times!
This was achieved by adding the `-O3` flag when compiling, which allows g++ to do optimization.

The v0.4.x releases will be focused on fixing bugs and evaluation algorithms.

## [Megalodon 0.3.2][v0.3.2]

There are many bug fixes in this release.

A new queen checkmate algorithm was added, and Megalodon will now use that algorithm instead of a search in queen endgames.

## [Megalodon 0.3.1][v0.3.1]

This release has move generation improvements.
The new algorithms reach 4.5 million nps, which is mostly due to using arrays instead of vectors.

There are also small evaluation weight changes.

The search function also runs in a different thread now, so the search can be stopped at any time with `stop`.

Megalodon plays at an estimated 1000 ELO.

## [Megalodon 0.3.0][v0.3.0]

This release features very fast move generation algorithms.
The move generation searches at 3.5 million NPS, a 50% improvement on 2.4 million in v0.2.6
The accuracy was also greatly improved, and agrees with the [Chess] module in python.

There are also minor evaluation changes.

[chess]: https://pypi.org/project/chess/

## [Megalodon 0.2.6][v0.2.6]

This release features many evaluation categories, including pawn structure, knights, kings, and space.

You can tune the weights with UCI options.

We will also start hosting a Sharktest server in a few days.

This allows you to use your CPU power to tune Megalodon's evaluation.

## [Megalodon 0.2.5][v0.2.5]

**THIS RELEASE CONTAINS BUGS**

This version is more than four times faster than the previous release.

There are also minor bug fixes, such as increasing the `nodes` max size.

The big speed increase is due to directly calling `eval` instead of `search` when there is one depth left.

The time management algorithm was also improved, and Megalodon will usually not time out or think very long.

You can also set when alpha-beta values are passed, and their margin through `setoption`.

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
[v0.4.1]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.4.1
[v0.4.0]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.4.0
[v0.3.2]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.3.2
[v0.3.1]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.3.1
[v0.3.0]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.3.0
[v0.2.6]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.6
[v0.2.5]: https://github.com/megalodon-chess/megalodon/releases/tag/v0.2.5
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
