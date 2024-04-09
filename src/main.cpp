#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
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
#include <SFML/Audio.hpp>
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
#include <thread>
#include <fstream>

#include <json/reader.h>
#include <json/writer.h>

#include "explosive.hpp"
#include "settings.hpp"
#include "gameobject.hpp"
#include "camera.hpp"
#include "globals.hpp"
#include "items.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "worldgen.hpp"
#include "gameScenes.hpp"
#include "rope.hpp"



Settings globalsettings = Settings();
sf::Time deltaTime;
sf::Time renderDeltaTime;
bool soundEnabled;
float fontScale = 1;

sf::Texture addTexture(std::string file) {
    sf::Texture text;
    if (!text.loadFromFile(file)) {
        std::cout << "Failed to load texture at: " << file;
        //Load default texture
        if (!text.loadFromFile("assets/sprites/missingTexture.png")) {
            std::cout << "Failed to load default texture aborting\n";
            abort();
        }
    }
    return text;
}

sf::SoundBuffer addSound(std::string file) {
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile(file)) {
        std::cout << "Failed to load sound at: " << file << " ABORTING";
        abort();
    }
    return buffer;
}

void setWindowFullScreen(bool fullscreen, sf::RenderWindow* window) {
    if (globalsettings.fullscreen)
        window->create(sf::VideoMode::getFullscreenModes()[0], "Spelunker", sf::Style::Fullscreen);
    else
        window->create(sf::VideoMode(sf::VideoMode::getDesktopMode().width/2, sf::VideoMode::getDesktopMode().height / 2), "Spelunker", sf::Style::Close);

    //Set resolution value in json and settings
    globalsettings.windowSize.x = window->getSize().x;
    globalsettings.windowSize.y = window->getSize().y;

    fontScale = float(globalsettings.windowSize.x / 1280.f);

    std::ifstream file("assets/settings.json");
    Json::Value settingsfile;
    Json::Reader reader;
    reader.parse(file, settingsfile);

    Json::Value& valueToEdit = settingsfile["windowSize"]["width"];
    valueToEdit = globalsettings.windowSize.x;
    valueToEdit = settingsfile["windowSize"]["height"];
    valueToEdit = globalsettings.windowSize.y;

    // Write back to the JSON file
    std::ofstream fileout("assets/settings.json");
    Json::StreamWriterBuilder builder;
    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    writer->write(settingsfile, &fileout);

};


int main()
{
    //Initialize window
    sf::RenderWindow window;
    setWindowFullScreen(globalsettings.fullscreen, &window);

    window.setVerticalSyncEnabled(globalsettings.vSync);
    window.setActive(true);
    window.setFramerateLimit(globalsettings.frameRateLimit);


    mainMenu menu = mainMenu(window);

    sf::Font font;
    if (!font.loadFromFile("assets/fonts/RubikScribble-Regular.ttf")) {
        std::cout << "Failed to load font";
    }




    gameRunning = false;


    //Load map of the games textures
    std::map<std::string, std::vector<sf::Texture*>> texturemap = {
        {"Player", {new sf::Texture(addTexture("assets/sprites/player/playerSheet.png"))}},
        {"Square", { new sf::Texture(addTexture("assets/sprites/square.jpg")) }},
        {"Dirt", {new sf::Texture(addTexture("assets/sprites/tiles/dirt/dirt.png")),new sf::Texture(addTexture("assets/sprites/tiles/dirt/dirt2.png")),new sf::Texture(addTexture("assets/sprites/tiles/dirt/dirt3.png"))}},
        {"Grass", {new sf::Texture(addTexture("assets/sprites/tiles/grass.png"))}},
        {"Dirtbottom", {new sf::Texture(addTexture("assets/sprites/tiles/dirtBottom.png"))}},
        {"Stone",{new sf::Texture(addTexture("assets/sprites/tiles/stone/stone.png")),new sf::Texture(addTexture("assets/sprites/tiles/stone/stone2.png"))}},
        {"Gold",{new sf::Texture(addTexture("assets/sprites/tiles/gold/gold.png")),new sf::Texture(addTexture("assets/sprites/tiles/gold/gold2.png"))}},
        {"Noomba",{new sf::Texture(addTexture("assets/sprites/noomba.png"))}},
        {"Bedrock",{new sf::Texture(addTexture("assets/sprites/tiles/bedrock.png"))}},
        {"Goldnugget",{new sf::Texture(addTexture("assets/sprites/goldNugget.png")),new sf::Texture(addTexture("assets/sprites/goldNugget2.png"))}},
        {"exitDoor",{new sf::Texture(addTexture("assets/sprites/exitDoor.png"))}},
        {"damageOverlay",{new sf::Texture(addTexture("assets/sprites/damageOverlay.png"))}},
        {"itemslot",{new sf::Texture(addTexture("assets/sprites/ui/itemSlot.png"))}},
        {"itemslotselected",{new sf::Texture(addTexture("assets/sprites/ui/itemSlotOverlay.png"))}},
        {"pickaxe",{new sf::Texture(addTexture("assets/sprites/ui/pickaxe.png"))}},
        {"whip",{new sf::Texture(addTexture("assets/sprites/ui/whip.png"))}},
        {"explosiveBarrel", {new sf::Texture(addTexture("assets/sprites/explosiveBarrel.png"))}},
        {"explosionSheet", {new sf::Texture(addTexture("assets/sprites/explosionSheet.png"))}},
        {"middleRope", {new sf::Texture(addTexture("assets/sprites/rope/middleRope.png")), new sf::Texture(addTexture("assets/sprites/rope/middleRope2.png")), new sf::Texture(addTexture("assets/sprites/rope/middleRope3.png"))}},
        {"endRope", {new sf::Texture(addTexture("assets/sprites/rope/ropeEnd.png"))}},
        {"startRope", {new sf::Texture(addTexture("assets/sprites/rope/ropeStart.png"))}},
        {"ropeProjectile", {new sf::Texture(addTexture("assets/sprites/rope/ropeProjectile.png"))}},
    };


    //Load a map of the games audio files (dont add more than 256 lol)
    soundmap = {
        {"tileHit",{new sf::SoundBuffer(addSound("assets/audio/stoneHit.wav"))}},
        {"tileBreak",{new sf::SoundBuffer(addSound("assets/audio/tileBreak.wav"))}},
        {"goldBreak",{new sf::SoundBuffer(addSound("assets/audio/goldBreak.wav"))}},
        {"footsteps",{new sf::SoundBuffer(addSound("assets/audio/footsteps/footstep1.wav")),new sf::SoundBuffer(addSound("assets/audio/footsteps/footstep2.ogg"))}},
        {"explosion",{new sf::SoundBuffer(addSound("assets/audio/explosion.wav"))}},
    };


    //Main menu UI

    ///Text elements
    uiElement* mainmenuTextElement = new uiElement(generateUIElement(font, int(150*fontScale), sf::Color(116, 12, 12), sf::Text::Bold, sf::Vector2<float>(50,-20), "Spelunker"));
    menu.uiElements.push_back(mainmenuTextElement);

    uiElement* playTextElement = new uiElement(generateUIElement(font, int(75 * fontScale), sf::Color(156, 51, 51), sf::Text::Bold, sf::Vector2<float>(50,200), "Start", true, true, sf::Color::White));
    playTextElement->onClickFunction = [&menu]() {
        menu.startGameTrigger = true;
    };
    menu.uiElements.push_back(playTextElement);

    uiElement* exitTextElement = new uiElement(generateUIElement(font, int(50 * fontScale), sf::Color(126, 10, 10), sf::Text::Bold, sf::Vector2<float>(50,300), "Exit", true, true, sf::Color::Red));
    exitTextElement->onClickFunction = [&window]() {
        window.close();
    };
    menu.uiElements.push_back(exitTextElement);

    uiElement* scoreElement = new uiElement(generateUIElement(font, int(80 * fontScale), sf::Color(126, 10, 10), sf::Text::Bold, sf::Vector2<float>(50, globalsettings.windowSize.y - (100.f * fontScale)), "Score: UNDEFINED", false));
    menu.uiElements.push_back(scoreElement);

    //Settings
    soundEnabled = true;
    uiElement* soundTextElement = new uiElement(generateUIElement(font, int(30 * fontScale), sf::Color(146, 30, 30), sf::Text::Bold, sf::Vector2<float>(globalsettings.windowSize.x-(250.f*fontScale), 50), "[X] Sound", true, true, sf::Color::Red));
    soundTextElement->onClickFunction = [&]() {
        if (soundEnabled) {
            soundEnabled = false;
            soundTextElement->text.setString("[   ] Sound");
        }
        else {
            soundEnabled = true;
            soundTextElement->text.setString("[X] Sound");
        }
    };
    menu.uiElements.push_back(soundTextElement);

    //fullscreenbutton
    uiElement* fullscreenTextElement = new uiElement(generateUIElement(font, int(30 * fontScale), sf::Color(146, 30, 30), sf::Text::Bold, sf::Vector2<float>(globalsettings.windowSize.x - (250.f*fontScale), 100), "[X] Fullscreen", true, true, sf::Color::Red));
    //Start in correct state
    if (!globalsettings.fullscreen) {
        fullscreenTextElement->text.setString("[   ] Fullscreen");
    }
    else {
        fullscreenTextElement->text.setString("[X] Fullscreen");
    }
    //OnClick
    fullscreenTextElement->onClickFunction = [&]() {
        if (globalsettings.fullscreen) {
            globalsettings.fullscreen = false;
            fullscreenTextElement->text.setString("[   ] Fullscreen");
        }
        else {
            globalsettings.fullscreen = true;
            fullscreenTextElement->text.setString("[X] Fullscreen");
        }
        //Set fullscreen value in json
        std::ifstream file("assets/settings.json");
        Json::Value settingsfile;
        Json::Reader reader;
        reader.parse(file, settingsfile);

        Json::Value& valueToEdit = settingsfile["fullscreen"];
        valueToEdit = globalsettings.fullscreen;

        // Write back to the JSON file
        std::ofstream fileout("assets/settings.json");
        Json::StreamWriterBuilder builder;
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        writer->write(settingsfile, &fileout);

        setWindowFullScreen(globalsettings.fullscreen, &window);


    };
    menu.uiElements.push_back(fullscreenTextElement);


    sf::Sound titlesound(*soundmap["tileHit"][0]);
    titlesound.play();

    exitState = 0; //State the last game exited with, 0 = nothing, 1 = won, 2 = died

    //Full loop
    while (window.isOpen()) {
        //Main menu loop
        menu.OnEvents();
        menu.OnLoop(window);
        menu.OnRender(window);

        //Remove straggling startgameTriggers when game already started
        if (gameRunning && menu.startGameTrigger) {
            menu.startGameTrigger = false;
        }

        //Level startup
        if (menu.startGameTrigger)
        {

            //Draw loading screen
            window.clear(globalsettings.backgroundColor);
            uiElement loadingText = generateUIElement(
                font, //Font
                50, //Size
                sf::Color::White, //Color 
                sf::Text::Bold,  //Style
                sf::Vector2<float>(0,0), //Position 
                "Loading..." //Text
            );
            window.draw(loadingText.text);
            window.display();

            //Set values

            gameRunning = true;
            menu.startGameTrigger = false;
            
            mainLevel game(window, texturemap.at("damageOverlay")[0]);
            gameScene = &game;

            //Set worldseed
            unsigned int seed = globalsettings.worldSeed;
            if (seed == 0) { // If seed is unset in the settings
                seed = (long)time(0); // Set random worldseed
            }
            currentSeed = seed;
            srand(seed);
            std::cout << "Starting game with seed: " << seed << "\n";

            //Load level

        #pragma region WorldGen
            //Tile types
            std::vector<tile> tileTypes = {
                tile("Air",10,Gameobject(),{nullptr},{nullptr}, false, treasureItem()),
                tile("Dirt",5,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Dirt"),true,true,globalsettings.gravity,1,0.2f,0.5f,sf::Vector2<float>(0,0)),texturemap.at("Grass"),texturemap.at("Dirtbottom"), false, treasureItem()),
                tile("Stone",10,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Stone"),true,true,globalsettings.gravity,1,0.2f,0.5f,sf::Vector2<float>(0,0)),{nullptr},{nullptr}, false, treasureItem()),
                tile("Gold",15,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Gold"),true,true,globalsettings.gravity,1,0.2f,0.7f,sf::Vector2<float>(0,0),"goldTile"),{nullptr},{nullptr}, true,
                    treasureItem(10,200,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(10,10),true,texturemap.at("Goldnugget"),false,true,globalsettings.gravity,20.f,0.2f,0,sf::Vector2<float>(0,0)))),
                tile("Bedrock",1000000,Gameobject(sf::Vector2<float>(0,0),0,sf::Vector2<float>(globalsettings.tileSize,globalsettings.tileSize),true,texturemap.at("Bedrock"),true,true,globalsettings.gravity,1,0.2f,0.5f,sf::Vector2<float>(0,0)),{nullptr},{nullptr}, false, treasureItem()),
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

            level world = level((int)globalsettings.tileSize, globalsettings.worldSize, tileTypes, 4, worldgenMap, replacementLayerMap);

            //Generate chunks
            std::vector<std::vector<chunk*>> chunks;
            for (int x = 0; x < globalsettings.worldSize.x; x++) {
                std::vector<chunk*> chunkcolomn;
                for (int y = 0; y < globalsettings.worldSize.y; y++) {
                    chunk* newchunk = new chunk(sf::Vector2<int>((int)(x * globalsettings.chunkSize * globalsettings.tileSize), (int)(y * globalsettings.chunkSize * globalsettings.tileSize)));
                    int expectedObjects = int(std::pow(globalsettings.chunkSize, 2) * 1.5);
                    newchunk->objects.reserve(expectedObjects);
                    newchunk->collisionObjects.reserve(expectedObjects);
                    chunkcolomn.push_back(newchunk);
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
                            if (_tile.position.y / globalsettings.tileSize - 1 > 0) {
                                if (world.tiles[(int)(_tile.position.x / globalsettings.tileSize)][(int)(_tile.position.y / globalsettings.tileSize - 1)].tileName == "Air") {
                                    _tile.sprite.setTexture(*_tile.topOverrideTexture, false);
                                }
                            }
                        }

                        if (_tile.bottomOverrideTexture != nullptr) {
                            //If below tile is air change texture
                            //Make sure not out of bounds
                            if (_tile.position.y / globalsettings.tileSize + 2 < world.tiles[(int)(_tile.position.x / globalsettings.tileSize)].size() * globalsettings.chunkSize) {
                                if (world.tiles[(int)(_tile.position.x / globalsettings.tileSize)][(int)(_tile.position.y / globalsettings.tileSize + 1)].tileName == "Air") {
                                    _tile.sprite.setTexture(*_tile.bottomOverrideTexture, false);
                                }
                            }
                        }
                    }




                    //Get corresponding chunk
                    sf::Vector2<int> chunkPos((int)(_tile.position.x / (globalsettings.chunkSize * globalsettings.tileSize)), (int)(_tile.position.y / (globalsettings.chunkSize * globalsettings.tileSize)));

                    //Add to chunk
                    chunks[chunkPos.x][chunkPos.y]->objects.push_back(&_tile);
                    if (_tile.hasCollision) {
                        chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(&_tile);
                    }
                }
            }

            sf::Vector2<int> tileWorldSize = globalsettings.worldSize * globalsettings.chunkSize;

            //Spawn exit Door
            bool spawnedDoor = false;

            int startX;
            int x = startX = rand() % tileWorldSize.x;

            int y = tileWorldSize.y - 2;

            while (!spawnedDoor) {
                //Check if not out of bounds
                if (x+1 < world.tiles.size())
                    if (y + 1 < world.tiles[x].size()) {

                        //Try to spawn door as low as possible
                        //Check for space
                        bool space = world.tiles[x][y].tileName == "Air" && world.tiles[x][y - 1].tileName == "Air" && world.tiles[x + 1][y].tileName == "Air" && world.tiles[x + 1][y - 1].tileName == "Air";
                        bool grounded = (world.tiles[x][y + 1].tileName != "Air" && world.tiles[x][y + 1].tileName != "Bedrock") && (world.tiles[x + 1][y + 1].tileName != "Air" && world.tiles[x + 1][y + 1].tileName != "Bedrock");
                        if (space && grounded) {
                            spawnedDoor = true;
                            //Spawn door
                            Gameobject* door = new Gameobject(sf::Vector2<float>(x * globalsettings.tileSize, (y - 1) * globalsettings.tileSize), 0, sf::Vector2<float>(globalsettings.tileSize * 2, globalsettings.tileSize * 2), true, texturemap.at("exitDoor"), true, true, 0, 0, 0, 0, sf::Vector2<float>(0, 0), "exit");

                            //Get corresponding chunk
                            sf::Vector2<int> chunkPos(x / (globalsettings.chunkSize), y / (globalsettings.chunkSize));
                            door->currentChunk = chunks[chunkPos.x][chunkPos.y];
                            chunks[chunkPos.x][chunkPos.y]->objects.push_back(door);
                            chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(door);
                            continue;
                        }
                }
                x++;
                if (x == startX || (startX == 0 && x == 1)) {
                    x = startX = rand() % tileWorldSize.x;
                    y--;
                    if (y <= 0) {
                        std::cout << "Failed to spawn exitdoor can not continue" << std::endl;
                        abort();
                    }
                }
                if (x >= tileWorldSize.x - 1)
                    x = 0;
            }



            //Spawn enemies
            int amountToSpawn = rand() % globalsettings.amountOfEnemies.y + globalsettings.amountOfEnemies.x;
            for (int i = 0; i < amountToSpawn; i++) {
                bool spawned = false;
                for (int j = 0; j < globalsettings.optionalObjectSpawnMaxIterations && !spawned; i++) {
                    int x = rand() % tileWorldSize.x;
                    int y = rand() % tileWorldSize.y;
                    //Check if tile position is empty
                    if (world.tiles[x][y].tileName == "Air" && world.tiles[x][y + 1].tileName != "Air") {
                        //Spawn enemy
                        Enemy* enemy = new Enemy(Gameobject(sf::Vector2<float>(x * globalsettings.tileSize, y * globalsettings.tileSize), 0, sf::Vector2<float>(32, 32), true, texturemap.at("Noomba"), false, true, globalsettings.gravity, 0.f, 0, 0.2f, sf::Vector2<float>(0, 0), "enemy"), 10, 50, 1, 1);
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

            //Spawn random objects
            if (globalsettings.amountOfObjects.x <= 0) amountToSpawn = 0;
            else amountToSpawn = rand() % globalsettings.amountOfObjects.y + globalsettings.amountOfObjects.x;
            for (int i = 0; i < amountToSpawn; i++) {
                Gameobject* object = nullptr;

                int rng = rand() % 2;

                int x = rand() % tileWorldSize.x;
                int y = rand() % tileWorldSize.y;

                bool spawned = false;
                for (int j = 0; j < globalsettings.optionalObjectSpawnMaxIterations && !spawned; j++) {

                    if (y < world.tiles[x].size()) {
                        //Check if tile position is empty
                        if (rng == 0) {
                            if (world.tiles[x][y].tileName == "Air" && world.tiles[x][y + 1].tileName != "Air") {
                                //Spawn object
                                object = new explosiveObject(3, 200, 20, 0.1f, 13, texturemap.at("explosionSheet")[0], Gameobject(sf::Vector2<float>(x * globalsettings.tileSize, y * globalsettings.tileSize), 0, sf::Vector2<float>(32, 32), true, texturemap.at("explosiveBarrel"), false, true, globalsettings.gravity, 0.f, 0.3f, 1, sf::Vector2<float>(0, 0), "explosiveBarrel"));

                                spawned = true;
                            }
                        }
                        else if (rng == 1) {
                            if (world.tiles[x][y].tileName == "Air" && world.tiles[x][y - 1].tileName != "Air") {
                                object = new Rope(rand() % 10, 0.1f, texturemap.at("middleRope"), Gameobject(sf::Vector2<float>(x * globalsettings.tileSize, y * globalsettings.tileSize), 0, sf::Vector2<float>(32, 32), true, texturemap.at("endRope"), true, true, 0, 0, 0, 0, sf::Vector2f(0, 0), "Rope"), true, texturemap.at("startRope"));

                                spawned = true;
                            }
                        }
                    }
                    x++;
                    if (x >= tileWorldSize.x) {
                        y++;
                        x = 0;
                        if (y >= tileWorldSize.y)
                            y = 0;
                    }

                }
                if (spawned) {
                    //Get corresponding chunk
                    sf::Vector2<int> chunkPos(x / (globalsettings.chunkSize), y / (globalsettings.chunkSize));
                    object->currentChunk = chunks[chunkPos.x][chunkPos.y];
                    chunks[chunkPos.x][chunkPos.y]->objects.push_back(object);
                    if (object->hasCollision)
                        chunks[chunkPos.x][chunkPos.y]->collisionObjects.push_back(object);
                }
            }


#pragma endregion

            //Spawn player

            Player player = Player(
                globalsettings.playerMoveSpeed,
                globalsettings.jumpVelocity,
                sf::FloatRect(0, 0, 15, 30),
                sf::Vector2<float>(40, 40),
                sf::Vector2<float>(-12.5, -10),
                RopeProjectile(
                    Rope(globalsettings.playerRopeSize, 0.5f, texturemap.at("middleRope"), Gameobject(sf::Vector2<float>(0, 0), 0, sf::Vector2<float>(32, 32), true, texturemap.at("endRope"), true, true, 0, 0, 0, 0, sf::Vector2f(0, 0), "Rope"), true, texturemap.at("startRope")),
                    Gameobject(sf::Vector2f(0, 0), 0, sf::Vector2f(16, 16), true, texturemap.at("ropeProjectile"), false, true, 0, 0, 0, 0, sf::Vector2f(0, 0), "ropeProjectile")
                ),
                Gameobject
                (
                    sf::Vector2<float>(0, 0),
                    0,
                    sf::Vector2<float>(30, 30),
                    true, texturemap.at("Player"),
                    false,
                    true,
                    globalsettings.gravity,
                    globalsettings.playerFriction,
                    0,
                    globalsettings.fallDamage,
                    sf::Vector2<float>(50, 0),
                    "Player"
                )
            );

            sf::Vector2<int> margin(30, 5);

            player.position.x = margin.x * globalsettings.tileSize;
            player.position.y = margin.y * globalsettings.tileSize;

            //Check for spot for player
            bool spawned = false;
            while (!spawned) {
                int x = (int)(player.position.x / globalsettings.tileSize);
                int y = (int)(player.position.y / globalsettings.tileSize);
                std::string currentTile = world.tiles[x][y].tileName;
                std::string groundTile = world.tiles[x][y + 1].tileName;
                if (currentTile == "Air" && groundTile != "Air") {
                    spawned = true;
                }
                else {
                    player.position.x += globalsettings.tileSize;
                    if (x >= globalsettings.worldSize.x * globalsettings.chunkSize - 1) {
                        player.position.x = (float)margin.x;
                        player.position.y += globalsettings.tileSize;
                    }
                    if (y >= globalsettings.worldSize.y * globalsettings.chunkSize - 1) {
                        std::cout << "ERROR: Failed to spawn playerref";
                        window.close();
                        return 0;
                    }
                }
            }
            game.player = &player;
            game.mainCamera.SetObjectToFollow(&player, 2);
            sf::Texture tileBreakTexture = addTexture("assets/sprites/tileBreakSheet.png");
            game.mainCamera.tileBreakTexture = &tileBreakTexture;



#pragma region UI

            uiElement fpsTextElement = generateUIElement(
                font, //Font
                int(24 * fontScale), //Size
                sf::Color::White, //Color 
                sf::Text::Bold,  //Style
                sf::Vector2<float>((float)(globalsettings.windowSize.x - (150*fontScale)), 0), //Position 
                "FPS" //Text
            );
            game.uiElements.push_back(&fpsTextElement);

            uiElement healthTextElement = generateUIElement(
                font, //Font
                int(34 * fontScale), //Size
                sf::Color::Red, //Color 
                sf::Text::Bold,  //Style
                sf::Vector2<float>(0, 0), //Position 
                "HP" //Text
            );
            game.uiElements.push_back(&healthTextElement);

            uiElement goldTextElement = generateUIElement(
                font, //Font
                int(30 * fontScale), //Size
                sf::Color::Yellow, //Color 
                sf::Text::Bold,  //Style
                sf::Vector2<float>(0, 30 * fontScale), //Position 
                "GOLD" //Text
            );
            game.uiElements.push_back(&goldTextElement);

            //Kinda a brute force way but we just have a couple slots so it does not matter as much
            ///Image elements

        //INVENTORY

            uiSprite inventoryslot1 = generateUISprite(texturemap.at("itemslot")[0], sf::Vector2<float>(10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&inventoryslot1);
            uiSprite pickaxe = generateUISprite(texturemap.at("pickaxe")[0], sf::Vector2<float>(10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&pickaxe);

            uiSprite inventoryslot2 = generateUISprite(texturemap.at("itemslot")[0], sf::Vector2<float>(10 + (100 * fontScale) + 10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&inventoryslot2);
            uiSprite whip = generateUISprite(texturemap.at("whip")[0], sf::Vector2<float>(10 + (100 * fontScale) + 10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&whip);

            uiSprite inventoryslot3 = generateUISprite(texturemap.at("itemslot")[0], sf::Vector2<float>(10 + 2 * (100 * fontScale) + 20, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&inventoryslot3);
            uiSprite rope = generateUISprite(texturemap.at("ropeProjectile")[0], sf::Vector2<float>(10 + 2 * (100 * fontScale) + 20, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&rope);

            uiSprite inventoryoverlay1 = generateUISprite(texturemap.at("itemslotselected")[0], sf::Vector2<float>(10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, true);
            game.uiSprites.push_back(&inventoryoverlay1); // 6
            uiSprite inventoryoverlay2 = generateUISprite(texturemap.at("itemslotselected")[0], sf::Vector2<float>(10 + (100 * fontScale) + 10, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, false);
            game.uiSprites.push_back(&inventoryoverlay2); // 7
            uiSprite inventoryoverlay3 = generateUISprite(texturemap.at("itemslotselected")[0], sf::Vector2<float>(10 + 2 * (100 * fontScale) + 20, (float)(globalsettings.windowSize.y - (100 * fontScale) - 10)), sf::Vector2<float>(100, 100) * fontScale, false);
            game.uiSprites.push_back(&inventoryoverlay3); // 8


        #pragma endregion

            //Game Loop
            std::thread gameLoopThread = std::thread([&]() {
                while (gameRunning) {
                    game.OnEvents();
                    game.OnLoop(window);
                }
            });

            //Rendering and window events
            while (gameRunning) {
                //Calculate camera
                game.mainCamera.OnLoop(window);

                window.clear(globalsettings.backgroundColor);
                game.OnRender(window);
                window.display();

                sf::Event event;
                while (window.pollEvent(event))
                {

                    if (event.type == sf::Event::Closed) {
                        if (gameRunning) {
                            gameRunning = false;
                        }
                        window.close();
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
                    if (gameRunning) {
                        gameRunning = false;
                    }else {
                        window.close();
                    }
                }
            }
            //Wait for thread
            gameLoopThread.join();

            //End game
            if (exitState == 0) {
                menu.uiElements[0]->text.setString("Spelunker");
                menu.uiElements[3]->enabled = false;
            }else
                menu.uiElements[3]->enabled = true;

            menu.uiElements[3]->text.setString("Score: "+std::to_string(dynamic_cast<Player*>(game.player)->gold));

            if (exitState == 1) { // Win
                menu.uiElements[0]->text.setString("YOU WON!");
            }else if (exitState == 2) { // Lose
                menu.uiElements[0]->text.setString("You died.");
            }

        }

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }


    while (window.isOpen())
    {
    }

    return 0;
}

//For windows application
int WinMain() {
    main();
}


