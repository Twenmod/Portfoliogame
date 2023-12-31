#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/System/String.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "gameobject.hpp"
#include "camera.hpp"
#include "globals.hpp"
#include "items.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "settings.hpp"
#include "worldgen.hpp"
#include <SFML/Window/WindowStyle.hpp>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

sf::Vector2<int> resolution(200,200);

Settings globalsettings = Settings();
sf::Time deltaTime;

//The game itself
class app {
    public:
        Camera mainCamera;

        std::vector<uiElement*> uiElements;

        std::vector<std::vector<chunk*>> chunkList;
        std::vector<chunk*> activeChunkList;
        
        sf::Clock gameClock; 

        sf::RenderWindow* gameWindow;

        //Chunks
        sf::Vector2<int> playerChunkPosition;

        Gameobject* player;

        //Fps calculator
        int fpsI = 0;
        float fpsArray[10];

        //Constructer/Init
        app(sf::RenderWindow &window) {
            gameWindow = &window;
            mainCamera = generateCamera(sf::Vector2<float>(0,0), sf::Vector2<float>(1,1));
        }

        Camera generateCamera(sf::Vector2<float> startPosition, sf::Vector2<float> scale) {
            return Camera(startPosition,scale,gameWindow->getSize());
        }

        void OnEvents() {
            for (chunk* _chunk : activeChunkList) {
                _chunk->OnEvents();
            }
        };
        void OnLoop(sf::RenderWindow &window) {
            //Get time elapsed since last frame
            deltaTime = gameClock.restart();
            deltaTime = sf::Time(sf::seconds(std::clamp(deltaTime.asSeconds(),0.0f,globalsettings.maxDeltaTime)));
            
            //Calculate camera
            mainCamera.OnLoop(window);

            //Get active chunks
            sf::Vector2<int> _playerchunkPos;
            _playerchunkPos.x = player->position.x/globalsettings.tileSize/globalsettings.chunkSize;
            _playerchunkPos.y = player->position.y/globalsettings.tileSize/globalsettings.chunkSize;
            if (_playerchunkPos != playerChunkPosition) {
                //Player moved to new chunk so update active chunks
                playerChunkPosition = _playerchunkPos;
                activeChunkList.clear();
                for (int x = -globalsettings.chunkLoadDistance; x < globalsettings.chunkLoadDistance+1; x++) {
                    for (int y = -globalsettings.chunkLoadDistance; y < globalsettings.chunkLoadDistance+1; y++) {
                        int chunkposx = _playerchunkPos.x + x;
                        int chunkposy = _playerchunkPos.y + y;
                        //Check if not out of bounds
                        if (!((chunkposx < 0 || chunkposx > globalsettings.worldSize.x-1) || (chunkposy < 0 || chunkposy > globalsettings.worldSize.y-1))) {
                            activeChunkList.push_back(chunkList[chunkposx][chunkposy]);
                        }
                    }
                }
            }

            //Calculate Objects
            for (chunk* _chunk : activeChunkList) {
                _chunk->OnLoop(activeChunkList, chunkList);
            }

            //Calculate Player
            player->OnLoop(activeChunkList);



            //Set UI
            fpsArray[fpsI] = 1/deltaTime.asSeconds();
            fpsI++;
            int fpsArraySize = std::size(fpsArray);
            if (fpsI >= fpsArraySize)
                fpsI = 0;
            float avarageFps = 0;
            for (int i = 0; i < fpsArraySize; i++) {
                avarageFps += fpsArray[i];
            }
            avarageFps /= fpsArraySize;
            uiElements[0]->text.setString("FPS: " + std::to_string((int)avarageFps));

            if (Player *_player = dynamic_cast<Player*>(player)) {
                uiElements[1]->text.setString("Health: " + std::to_string((int)_player->health) + "/" + std::to_string((int)_player->maxHealth));
                uiElements[2]->text.setString("Gold: " + std::to_string(_player->gold));
            }

        };
        void OnRender(sf::RenderWindow &window) {
            mainCamera.Render(window, player, activeChunkList, uiElements);
        };
};


sf::Texture addTexture(std::string file) {
    sf::Texture text;
    if (!text.loadFromFile(file)) {
        std::cout << "Failed to load texture at: " << file;
        //Load default texture
        if (!text.loadFromFile("sprites/nobitches.png")) {
            std::cout << "Failed to load default texture aborting\n";
            abort();
        }
    }
    return text;
}

int main()
{
    //Initialize window
    sf::RenderWindow window(sf::VideoMode((int)globalsettings.windowSize.x,(int)globalsettings.windowSize.y), "Minergame", sf::Style::Close);

    window.setVerticalSyncEnabled(globalsettings.vSync);
    window.setActive(true);
    window.setFramerateLimit(globalsettings.frameRateLimit);

    //Random seed
    srand(globalsettings.worldSeed);
    std::cout << "Starting game with seed: " << globalsettings.worldSeed << "\n";


    //Map of the games textures

    std::map<std::string, std::vector<sf::Texture*>> texturemap = {
        {"Player", {new sf::Texture(addTexture("sprites/player/playerSheet.png"))}},
        {"Square", { new sf::Texture(addTexture("sprites/square.jpg")) }},
        {"Dirt", {new sf::Texture(addTexture("sprites/tiles/dirt/dirt.png")),new sf::Texture(addTexture("sprites/tiles/dirt/dirt2.png")),new sf::Texture(addTexture("sprites/tiles/dirt/dirt3.png"))}},
        {"Grass", {new sf::Texture(addTexture("sprites/tiles/grass.png"))}},
        {"Dirtbottom", {new sf::Texture(addTexture("sprites/tiles/dirtBottom.png"))}},
        {"Stone",{new sf::Texture(addTexture("sprites/tiles/stone/stone.png")),new sf::Texture(addTexture("sprites/tiles/stone/stone2.png"))}},
        {"Gold",{new sf::Texture(addTexture("sprites/tiles/gold/gold.png")),new sf::Texture(addTexture("sprites/tiles/gold/gold2.png"))}},
        {"Noomba",{new sf::Texture(addTexture("sprites/noomba.png"))}},
        {"Bedrock",{new sf::Texture(addTexture("sprites/tiles/bedrock.png"))}},
        {"Goldnugget",{new sf::Texture(addTexture("sprites/goldnugget.png")),new sf::Texture(addTexture("sprites/goldnugget2.png"))}},
        {"exitDoor",{new sf::Texture(addTexture("sprites/exitDoor.png"))}},
    };

    //Load game
    app game(window);

    //Load level

#pragma region WorldGen
    //Tile types
    std::vector<tile> tileTypes = {
        tile("Air",10,Gameobject(),{nullptr},{nullptr}, false, treasureItem()),
        tile("Dirt",5,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Dirt"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0)),texturemap.at("Grass"),texturemap.at("Dirtbottom"), false, treasureItem()),
        tile("Stone",10,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Stone"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0)),{nullptr},{nullptr}, false, treasureItem()),
        tile("Gold",15,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Gold"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0)),{nullptr},{nullptr}, true,
            treasureItem(10,200,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(10,10),true,texturemap.at("Goldnugget"),false,true,globalsettings.gravity,100,0.4f,sf::Vector2<float>(0,0)))),
        tile("Bedrock",1000000,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Bedrock"),true,true,globalsettings.gravity,1,0.2,sf::Vector2<float>(0,0)),{nullptr},{nullptr}, false, treasureItem()),

    };

    //This map is used to determine the rough shape of the terain and places dirt and stone there
    //Int points to the type of tile to spawn if the noise is above the float the tile is used
    std::map<int, float> worldgenMap{
        {0,0.f},
        {1,0.5f},
        {2,0.6f},
    };

    //This map is used to add more detail to the terrain by replacing certain tiles with other tiles based on noise
    //Int points to type to replace if the noise is above the float, the second int dictates which tile to place there
    std::map<int, std::pair<float, int>> replacementLayerMap{
        {2, std::pair<float, int>(0.75f, 3)},
    };

    level world = level(globalsettings.tileSize,globalsettings.worldSize, tileTypes, 4, worldgenMap,replacementLayerMap);
    
    //Generate chunks
    std::vector<std::vector<chunk*>> chunks;
    for (int x = 0; x < globalsettings.worldSize.x; x++) {
        std::vector<chunk*> chunkcolomn;
        for (int y = 0; y < globalsettings.worldSize.y; y++) {
            chunkcolomn.push_back(new chunk(sf::Vector2<int>(x*globalsettings.chunkSize*globalsettings.tileSize,y*globalsettings.chunkSize*globalsettings.tileSize)));
        }
        chunks.push_back(chunkcolomn);
    }

    //Loop to all tiles and add them to chunks and change textures
    for (std::vector<tile>& tilecolomn : world.tiles) {
        for (tile& _tile : tilecolomn) {
            if (_tile.tileName != "Air") {
                //Change tile textures based on tiles around
                if (_tile.topOverrideTexture != nullptr) {
                    //If above tile is air change texture
                    //Make sure not out of bounds
                    if (_tile.position.y/globalsettings.tileSize - 1 > 0) {
                        if(world.tiles[_tile.position.x/globalsettings.tileSize][_tile.position.y/globalsettings.tileSize - 1].tileName == "Air") {
                            _tile.sprite.setTexture(*_tile.topOverrideTexture, false);
                        }
                    }
                }

                if (_tile.bottomOverrideTexture != nullptr) {
                    //If below tile is air change texture
                    //Make sure not out of bounds
                    if (_tile.position.y/globalsettings.tileSize + 1 < globalsettings.worldSize.y*globalsettings.chunkSize) {
                        if(world.tiles[_tile.position.x/globalsettings.tileSize][_tile.position.y/globalsettings.tileSize + 1].tileName == "Air") {
                            _tile.sprite.setTexture(*_tile.bottomOverrideTexture, false);
                        }
                    }
                }
            }

            


            //Get corresponding chunk
            sf::Vector2<int> chunkPos(_tile.position.x / (globalsettings.chunkSize * globalsettings.tileSize), _tile.position.y / (globalsettings.chunkSize * globalsettings.tileSize));
            
            //Add to chunk
            chunks[chunkPos.x][chunkPos.y]->objects.push_back(&_tile);
            if(_tile.hasCollision) {
                chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(&_tile);
            }
         }
    }

    sf::Vector2<int> tileWorldSize = globalsettings.worldSize * globalsettings.chunkSize;

    //Spawn exit Door
    bool spawnedDoor = false;
    
    int startX;
    int x = startX = rand() % tileWorldSize.x;
    int y = tileWorldSize.y-2;

    while (!spawnedDoor) {
            //Try to spawn door as low as possible
            //Check for space
            bool space = world.tiles[x][y].tileName == "Air" && world.tiles[x][y-1].tileName == "Air" && world.tiles[x+1][y].tileName == "Air" && world.tiles[x+1][y-1].tileName == "Air";
            bool grounded = (world.tiles[x][y+1].tileName != "Air" && world.tiles[x][y+1].tileName != "Bedrock") && (world.tiles[x+1][y+1].tileName != "Air" && world.tiles[x+1][y+1].tileName != "Bedrock");
            if (space && grounded) {
                spawnedDoor = true;
                //Spawn door
                Gameobject* door = new Gameobject(sf::Vector2<float>(x*globalsettings.tileSize,(y-1)*globalsettings.tileSize),0,sf::Vector2<float>(globalsettings.tileSize*2,globalsettings.tileSize*2),true,texturemap.at("exitDoor"),true,true,0,0,0,sf::Vector2<float>(0,0),"exit");
                
                //Get corresponding chunk
                sf::Vector2<int> chunkPos(x / (globalsettings.chunkSize), y / (globalsettings.chunkSize));
                door->currentChunk = chunks[chunkPos.x][chunkPos.y];
                chunks[chunkPos.x][chunkPos.y]->objects.push_back(door);
                chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(door);
            }else {
                x++;
                if (x == startX || (startX == 0 && x == 1)) {
                    x = startX = rand() % tileWorldSize.x;
                    y--;
                    if (y <= 0) {
                        std::cout << "Failed to spawn exitdoor can not continue" << std::endl;
                        abort();
                    }
                }
                if (x >= tileWorldSize.x-1)
                    x = 0;
            }


    }



    //Spawn enemies
    int amountToSpawn = rand() % globalsettings.amountOfEnemies.y + globalsettings.amountOfEnemies.x;
    for (int i = 0; i < amountToSpawn; i++) {
        bool spawned = false; 
        while (!spawned) {
            int x = rand() % tileWorldSize.x;
            int y = rand() % tileWorldSize.y;
            //Check if tile position is empty
            if (world.tiles[x][y].tileName == "Air") {
                //Spawn enemy
                Enemy* enemy = new Enemy(10,50,Gameobject(sf::Vector2<float>(x*globalsettings.tileSize,y*globalsettings.tileSize),0,sf::Vector2<float>(32,32),true,texturemap.at("Noomba"),false,true,globalsettings.gravity,0.f,0, sf::Vector2<float>(0,0)));
                //Get corresponding chunk
                sf::Vector2<int> chunkPos(x / (globalsettings.chunkSize), y / (globalsettings.chunkSize));
                enemy->currentChunk = chunks[chunkPos.x][chunkPos.y];
                chunks[chunkPos.x][chunkPos.y]->objects.push_back(enemy);
                if (enemy->hasCollision)
                    chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(enemy);

                spawned = true;
            }
        }
    }

    game.chunkList = chunks;
    globalChunkList = chunks;


#pragma endregion

    //Spawn player

    Player player = Player(globalsettings.playerMoveSpeed,globalsettings.jumpVelocity,sf::FloatRect(0,0,15,30), sf::Vector2<float>(40,40),sf::Vector2<float>(-12.5,-10),Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(30,30),true,texturemap.at("Player"),false,true,globalsettings.gravity,globalsettings.playerFriction,0, sf::Vector2<float>(50,0)));

    sf::Vector2<int> margin(30,5);

    player.position.x = margin.x * globalsettings.tileSize;
    player.position.y = margin.y * globalsettings.tileSize;

    //Check for spot for player
    bool spawned = false;
    while (!spawned) {
        int x = player.position.x / globalsettings.tileSize;
        int y = player.position.y / globalsettings.tileSize;
        std::string currentTile = world.tiles[x][y].tileName;
        std::string groundTile = world.tiles[x][y+1].tileName;
        if (currentTile == "Air" && groundTile != "Air") {
            spawned = true;
        }else {
            player.position.x += globalsettings.tileSize;
            if (x >= globalsettings.worldSize.x*globalsettings.chunkSize-1) {
                player.position.x = margin.x;
                player.position.y += globalsettings.tileSize;
            }
            if (y >= globalsettings.worldSize.y*globalsettings.chunkSize-1) {
                std::cout << "ERROR: Failed to spawn playerref";
                window.close();
                return 0;
            }
        }
    }
    game.player = &player;
    game.mainCamera.SetObjectToFollow(&player, 2);
    sf::Texture tileBreakTexture = addTexture("sprites/tileBreakSheet.png");
    game.mainCamera.tileBreakTexture = &tileBreakTexture;

#pragma region UI

    sf::Font font;
    if (!font.loadFromFile("Fonts/RubikScribble-Regular.ttf")) {
        std::cout << "Failed to load font";
    }


    //FPS text
    sf::Text fpstext;
    fpstext.setCharacterSize(24);
    fpstext.setFillColor(sf::Color::White);
    fpstext.setStyle(sf::Text::Bold);
    fpstext.setPosition(globalsettings.windowSize.x-150,0);

    uiElement fpstextElement(fpstext,font);
    game.uiElements.push_back(&fpstextElement);

    //Health text
    sf::Text hptext;
    hptext.setCharacterSize(24);
    hptext.setFillColor(sf::Color::Red);
    hptext.setStyle(sf::Text::Bold);
    hptext.setPosition(sf::Vector2<float>(0,0));

    uiElement healthElement(hptext,font);
    game.uiElements.push_back(&healthElement);

    //Gold Text
    sf::Text goldtext;
    goldtext.setCharacterSize(24);
    goldtext.setFillColor(sf::Color::Yellow);
    goldtext.setStyle(sf::Text::Bold);
    goldtext.setPosition(sf::Vector2<float>(0,24));

    uiElement goldElement(goldtext,font);
    game.uiElements.push_back(&goldElement);

#pragma endregion

    while (window.isOpen())
    {

        //Run the game loop
        game.OnEvents();
        game.OnLoop(window);
        game.OnRender(window);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
            window.close();
        }

        window.display();
        window.clear();

    }

    return 0;
}

