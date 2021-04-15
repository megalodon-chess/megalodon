![logo](https://raw.githubusercontent.com/megalodon-chess/megalodon/main/logo/logo_widescreen_light.png)

# Megalodon

UCI chess engine.

Play against Megalodon on [Lichess][lichess]!

[Version 1.0.0][latest] was recently released!

## Compiling

### Compiling with automated Bash Script

Requires g++, CMake, and GNU Make.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git    # Clone this repository to your local device
cd ./megalodon/                                               # change directory into the ./megalodon/ directory
./build.sh                                                    # Run the bash script
./build/Megalodon                                             # The executable will end up in the ./build/ directory inside the megalodon directory
```

### Compiling with CMake and GNU Make

Requires g++, CMake, and GNU Make.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git    # Clone this repository to your local device
cd ./megalodon/                                               # change directory into the ./megalodon/ directory
mkdir ./build/                                                # Create a build directory where all the build contents will be stored
cd ./build/                                                   # change directory into the build directory
cmake ..                                                      # Create a Makefile with CMake
make -j                                                       # Compile with flag: -j(optional, uses all cores for speed)
./Megalodon                                                   # Megalodon will be located in the ./build/ directory, run it.
```

### Compiling Manually

Requires g++.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git    # Clone this repository to your local device
cd ./megalodon/src/                                           # change directory into the ./megalodon/ directory and ./src/ directory at once
g++ -pthread -Ofast *.cpp -o Megalodon                        # Compile using g++ with flags: -pthread(for threading), -Ofast(speed during runtime), -o(name of executable)
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
