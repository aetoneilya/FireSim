#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "AnimationManager.h"
#include "Fire.h"
#include "Human.h"
#include "TmxLevel.h"
#include "VectorFlow.h"

// TODO: fix path check
// FiX fire outside/walls

class Engine {
   private:
    std::string name;
    std::pair<int, int> resoluttion;
    sf::RenderWindow window;
    sf::View camera;
    sf::Vector2f cameraPos;
    float cameraSpeed;

    TmxLevel level;
    std::vector<std::vector<int>> collisionMap;
    TmxObject player;

    std::vector<TmxObject> walls;
    std::vector<TmxObject> exits;

    sf::Texture humanText, fireText;
    std::list<Human> humans;
    std::list<Fire> fires;

    float timer;
    void fireSpread();

    static sf::Vector2f GetPlayerDirection();

   public:
    Engine(std::string projectName, std::pair<int, int> res) : camera(sf::FloatRect(0, 0, 800, 600)) {
        name = projectName;
        resoluttion = res;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode(resoluttion.first, resoluttion.second), name, sf::Style::Close, settings);
        window.setFramerateLimit(60);

        cameraSpeed = 200.f;
        cameraPos = {(float)resoluttion.first, (float)resoluttion.second};
        timer = 0;
    }
    void loadGameScene();
    void startEngine();
    void update(float elapsedTime);
    void draw();

    void fillCollisionMap();
};