#include "worldgen.hpp"
#include "gameobject.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <vector>
#include "perlinnoise.hpp"
#include "settings.hpp"


//Constructor
tile::tile(sf::String _tileName, float _tileHealth, Gameobject tileObject) : Gameobject(tileObject) {
    tileName = _tileName;
    maxHealth = _tileHealth;
    health = _tileHealth;
}

level::level(int tileGridSize ,sf::Vector2<int> worldsize, std::vector<tile> tileTypes, std::map<int, float> noiseTileMap) {
    
    //Generate perlin
    const siv::PerlinNoise::seed_type seed = random();
    const siv::PerlinNoise perlin { seed };




    //Spawn all tiles
    for (int x = 0; x < worldsize.x; x++) {
        std::vector<tile> column;
        for (int y = 0; y < worldsize.y; y++) {
            //Get noise
			const double noise = perlin.octave2D_01((x * globalsettings.noiseScale.x), (y * globalsettings.noiseScale.y), globalsettings.octaves, globalsettings.persistence);

            int type = -1;
            for(auto const& mapLocation : noiseTileMap) {
                if (noise > mapLocation.second) { // If noise value is falls bellow location
                    type = mapLocation.first; // Set the type to type of that map location
                }
            }

            //Spawn corresponding tile
            if (type != -1) {
                tile currTile = tileTypes[type];
                currTile.position.x = x*tileGridSize;
                currTile.position.y = y*tileGridSize;
                column.push_back(currTile);
                std::cout <<"spawning tile at x: " << x << " y: " << y << std::endl;
            }
        }
        tiles.push_back(column);
    }

}
