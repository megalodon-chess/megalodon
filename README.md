![logo](https://raw.githubusercontent.com/megalodon-chess/megalodon/main/logo/logo_widescreen_light.png)

# Megalodon

UCI chess engine.

Play against Megalodon on [Lichess][lichess]!

[Version 0.2.5][latest] was recently released!

## Compiling

### Requirements

* GNU Make
* CMake

### With Bash Script

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
./build.sh
./build/Megalodon
```

### Manually

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
mkdir build
cd build/
cmake ..
make -j
./Megalodon
```

## License

Everything in the repository is licensed as GNU GPL v3 license.

If you would like to use the logo, we request that you also include a message
saying that your project isn't related to Megalodon.

## The Name

The name megalodon comes from a species of shark that lived millions of years ago.

## Documentation

For extra documentation, please read the [docs][docs].

[lichess]: https://lichess.org/@/megalodon-chess
[docs]: https://megalodon-chess.github.io/megalodon/
[latest]: https://github.com/megalodon-chess/megalodon/releases/latest
