# Conway's Game of Life

A C++ / SDL3 implementation of Conway's Game of Life.

## To Install:

1. Ensure you have cmake installed on your system.
2. Ensure you have downloaded the SDL submodule, by doing `git submodule update --init --recursive`
3. Make a build directory: `mkdir build`
4. Initialize cmake: `cd build`, then `cmake ..` (this will take a long time the first time, because it is compiling SDL as well as the game itself)
5. Make the target: `make`
6. Run the target: `./life`
