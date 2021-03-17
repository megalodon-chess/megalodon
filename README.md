![logo](https://raw.githubusercontent.com/megalodon-chess/megalodon/main/logo/logo_widescreen_light.png)

# Megalodon

UCI chess engine.

[Version 0.2.0][latest] was recently released!

## To compile with script

1. Clone or download the repository.
2. `cd` to `megalodon/`
4. Run `build.sh`
5. Run the executable inside `build/`

```bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
./build.sh
./build/Megalodon
```

## To compile manually

1. Clone or download the repository.
2. `cd` to `megalodon/`
3. Create `build` directory.
4. `cd` to `build/`
5. Create `Makefile` with `cmake`
6. Use `make -j` to create the executable with all cores.
7. Run the executable.

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
mkdir build
cd build/
cmake ..
make -j
./Megalodon
```

## To compile and run
1. Clone or download the repository.
2. `cd` to `megalodon/`
3. Run `run.sh`

```bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
./run.sh
```

## License

Everything in the repository is licensed as GNU GPL v3 license.

If you would like to use the logo, we request that you also include a message
saying that your project isn't related to Megalodon.

## The Name

The name megalodon comes from a species of shark that lived millions of years ago.

## Documentation

For extra documentation, please read the [docs][docs].

[docs]: https://megalodon-chess.github.io/megalodon/
[latest]: https://github.com/megalodon-chess/megalodon/releases/latest
