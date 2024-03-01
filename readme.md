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
`~$` `sudo apt install sfml-dev libjsoncpp-dev`

or downloading it from the aur on arch

### Windows

2. Install jsoncpp using vcpkg
`
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install jsoncpp
`
### Then

3. Built the cmake cache using `cmake .`
Note the project files only include the libraries for 32 bit systems so on windows you would have to do
`cmake . -A Win32`

4. Built the project using `cmake --build . --config Release`

# How to play

## controls
- Use [A] and [D] to walk around
- Use [Space] to jump
- Use [UP][LEFT][DOWN][RIGHT] to attack around you
- Press [E] to interact
- Press [ESC] to return to the main menu (lose progress)

## Gameplay
- Your objective is to collect as much treasure/gold as possible
- Collect gold by mining ore or ~~finding treasure~~
- Get to the exit point alive

## Tips
- Do not die

# Known Issues
- The physics are not accurate enough when <30 fps