# Spelunker
A mining game where you need to explore caves and mine to collect gold treasure.

Made for the intake assignement of the Breda University of Aplied Science
with the theme "collect"

# Installation

## Playing

- Download a release build for linux or windows in the [releases](https://github.com/Twenmod/Portfoliogame/releases) tab

- Run the binary/executable file and start playing the game

Note: Some windows 10 systems falsely flag the game as a virus if this is the case I would suggest building the project yourself using the instructions below 

## Building

### Windows

#### In visual studio (community 2022)

1. Download the project folder or use git clone from the terminal (do not use the visual studio git clone it acts opens the project in a weard way)
`git clone https://github.com/twenmod/portfoliogame`

2. Open the project folder with visual studio
  
3. Generate the cmake cache by clicking on `project>configure cache` from the topbar

4. Built the game (ctrl+b)


#### From terminal
1. Clone the repo
`git clone https://github.com/twenmod/portfoliogame`
  
2. Go into the project folder
  
3. Built the cmake cache using `cmake .`
Note the project files only include the libraries for 64 bit systems

4. Built the project using `cmake --build . --config Release`

### Linux

1. Clone the repo
`git clone https://github.com/twenmod/portfoliogame`

2. Make sure you have the dependencies installed by running
`sudo apt install cmake sfml-dev libjsoncpp-dev gcc`

or downloading it from the aur on arch

3. Build the game by running the "C/C++: g++ build the game" task from vscode or running 
`g++ -g src/*.cpp -I ./src -o ./out/build/gcc/Spelunker -L /usr/lib -I /usr/include -l sfml-graphics -l sfml-window -l sfml-system -l sfml-audio -l sfml-network -l jsoncpp -std=c++17`
from within the source folder



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
