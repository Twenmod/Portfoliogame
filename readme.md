# Spelunker
A mining game where you need to collect treasure similar to spelunky

Made for the intake assignement of the Breda University of Aplied Science
with the theme "collect"

# Installation

## Playing

- Download a release build for linux or windows in the [releases](https://github.com/Twenmod/Portfoliogame/releases) tab

- Run the binary/executable file and start playing the game

## Building

1. Clone the repository

### Linux

2. Make sure you have the dependencies installed by running
`sudo apt install sfml-dev libjsoncpp-dev gcc`

or downloading it from the aur on arch

3. Build the game by running the "C/C++: g++ build the game" task from vscode or running 
`g++ -g src/*.cpp -I ./src -o ./out/build/gcc/Spelunker -L /usr/lib -I /usr/include -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio -l sfml-network -l jsoncpp -std=c++17`
from within the source folder

### Windows

2. Built the cmake cache using `cmake .` or the popup from within visual studio
Note the project files only include the libraries for 64 bit install

4. Built the project using `cmake --build . --config Release` or the build button (ctrl+b) from within visual studio

# How to play

## controls
- Use [A] and [D] to walk around
- Use [Space] to jump
- Use [1][2] To select your weapon
- Use [UP][LEFT][DOWN][RIGHT] to attack around you
- Press [E] to interact (with doors)
- Press [ESC] to return to the main menu (lose progress)

## Gameplay
- Your objective is to collect as much treasure/gold as possible
- Collect gold by mining ore or ~~finding treasure~~
- Get to the exit point alive

## Tips
- Do not die

# Known Issues
- The physics are not accurate enough when <30 fps
