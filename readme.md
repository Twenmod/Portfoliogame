# Spelunker
A mining game where you need to explore caves and mine to collect gold treasure.

Made for the intake assignement of the Breda University of Aplied Science
with the theme "collect"

# Installation

## Playing

- Download a release build for linux or windows in the [releases](https://github.com/Twenmod/Portfoliogame/releases) tab

- Run the binary/executable file and start playing the game

## Building

1. Clone the repository

2. Make sure you have C++ and cmake installed

### Linux

3. Make sure you have the dependencies installed by running
`sudo apt install sfml-dev libjsoncpp-dev gcc`

or downloading it from the aur on arch

4. Build the game by running the "C/C++: g++ build the game" task from vscode or running 
`g++ -g src/*.cpp -I ./src -o ./out/build/gcc/Spelunker -L /usr/lib -I /usr/include -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio -l sfml-network -l jsoncpp -std=c++17`
from within the source folder

### Windows
## From terminal

3. Built the cmake cache using `cmake .`
Note the project files only include the libraries for 64 bit systems

4. Built the project using `cmake --build . --config Release`

## In visual studio

3. Open the project

4. Built using build solution or ctrl+shift+b

Important note: **make sure you select the "Spelunker.vcxproj" build file and not the ""ALL_BUILD.vcxproj"" file**

# How to play

## controls
- Use [A] and [D] to walk around
- Use [Space] or [W] to jump
- Use [1][2][3] To select your weapon/tool
- Use [UP][LEFT][DOWN][RIGHT] to attack around you (note not all weapons can attack in all directions)
- Press [E] to interact (with doors)
- Press [ESC] to return to the main menu (lose progress)

## Gameplay
- Your objective is to collect as much gold as possible
- Collect gold by mining ore
- Get to the exit door alive

## Tips
- Do not die

# Outside sources used
- SFML
- JsonCPP
- SFMLMath by Denis Wagner
- Perlinnoise generator by Ryo Suzuki
- Footsteps sound from GboxMikeFozzy on opengameart.opengameart
- Explosion sound from opengameart
- Player sprite made from a template
- Font from google fonts

All other scripts and assets are made by me


# Known Issues
- The physics of objects (not player) are not accurate enough when <30 fps
- The rope tool doesn't work at the spawn chunks
- Sometimes the player falls down when the level just loaded in.
