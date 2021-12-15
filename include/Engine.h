#pragma once
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "AnimationManager.h"
#include "Error.h"
#include "Fire.h"
#include "Human.h"
#include "LevelData.h"
#include "TmxLevel.h"
#include "VectorFlow.h"

class Engine {
   private:
    std::string name;
    std::pair<int, int> resoluttion;
    sf::RenderWindow window;
    sf::View camera;
    sf::Vector2f cameraPos;
    float cameraSpeed;
    float zoom;

    int humanCount;
    int humanMax;
    int safeCount;
    int deadCount;

    sf::SoundBuffer deathSoundBuf;
    sf::Sound deathSound;
    sf::SoundBuffer safeSoundBuf;
    sf::Sound safeSound;
    sf::SoundBuffer startFireSoundBuf;
    sf::Sound startFireSound;
    sf::SoundBuffer extinguisherSoundBuf;
    sf::Sound extinguisherSound;

    LevelData levelData;
    TmxLevel level;

    float waitHumanSpawn;

    sf::Text text;
    std::string string;
    sf::Font font;

    sf::Texture humanTxtr, fireTxtr, extingTxtr, btnTxtr, deadTxtr, postFireTxtr;
    std::list<Human> humans;
    std::list<Fire> fires;

    void fireSpread();
    static sf::Vector2f GetPlayerDirection();

   public:
    Engine(std::string projectName, std::pair<int, int> res) : camera(sf::FloatRect(0, 0, 400, 300)) {
        name = projectName;
        resoluttion = res;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode(resoluttion.first, resoluttion.second), name, sf::Style::Close, settings);
        window.setFramerateLimit(60);

        humanCount = 0;
        humanMax = 10;
        safeCount = 0;
        deadCount = 0;

        cameraSpeed = 200.f;
        zoom = 1;
        cameraPos = {(float)resoluttion.first, (float)resoluttion.second};
        levelData.fireTimer = 0;
        levelData.humanTimer = 0;
        waitHumanSpawn = 0;

        font.loadFromFile("../assets/AnkaCoder-C75-b.ttf");
        text.setFont(font);
        text.setCharacterSize(20);
    }
    void loadGameScene();
    void startEngine();
    void update(float elapsedTime);
    void draw();

    void addFire(sf::Vector2f worldPos);
    void fillCollisionMap();
    bool isIntersects(sf::FloatRect& rect, std::vector<TmxObject>& objects);
};