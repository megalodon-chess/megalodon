![logo](https://raw.githubusercontent.com/megalodon-chess/megalodon/main/logo/logo_widescreen_light.png)

# Megalodon

UCI chess engine.

Play against Megalodon on [Lichess][lichess]!

[Version 0.3.2][latest] was recently released!

# Compiling

## Requirements

* GNU Make
* CMake
* g++

## Compiling With Automated Bash Script

Requires g++, CMake, and GNU Make.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
./build.sh
./build/Megalodon
```

## Compiling With CMake and GNU Make

Requires g++, CMake, and GNU Make.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
mkdir build
cd build/
cmake ..
make -j
./Megalodon
```

## Compiling Manually

Requires g++.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/src/
g++ -pthread *.cpp -o Megalodon
./Megalodon
```

## License

Everything in the repository is licensed as GNU GPL v3 license.

If you would like to use the logo, we request that you also include a message
saying that your project isn't related to Megalodon.

## The Name

The name Megalodon comes from a species of shark that lived millions of years ago.

## Documentation

For extra documentation, please read the [docs][docs].

[lichess]: https://lichess.org/@/megalodon-chess
[docs]: https://megalodon-chess.github.io/megalodon/
[latest]: https://github.com/megalodon-chess/megalodon/releases/latest
