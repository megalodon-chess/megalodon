# Compiling Megalodon

To compile Megalodon from it's source code, follow these steps:

## Requirements

* GNU Make
* CMake

## Compiling With Bash Script

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
./build.sh
./build/Megalodon
```

## Compiling Manually

``` bash
git clone https://github.com/megalodon-chess/megalodon.git
cd megalodon/
mkdir build
cd build/
cmake ..
make -j
./Megalodon
```

[Back to documentation home][home]

[home]: https://megalodon-chess.github.io/megalodon/
