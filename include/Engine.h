#pragma once
#include <utility>
#include <string>

#include <SFML/Graphics.hpp>

#include "AnimationManager.h"
#include "TmxLevel.h"
#include "Human.h"
#include "Fire.h"
#include "TmxLevel.h"
#include "Astar.h"

class Engine
{
private:
    std::string name;
    std::pair<int, int> resoluttion;
    sf::RenderWindow window;
    sf::View camera;

    TmxLevel level;
    std::vector<std::vector<int>> collisionMap;
    TmxObject player;

    std::vector<TmxObject> walls;

    sf::Texture humanText, fireText;
    std::vector<Human> humans;
    std::vector<Fire> fires;
    
    Astar pathfind;

public:
    Engine(std::string projectName, std::pair<int, int> res)
    {
        name = projectName;
        resoluttion = res;

        sf::ContextSettings settings;
        settings.antialiasingLevel = 8;
        window.create(sf::VideoMode(resoluttion.first, resoluttion.second), name,
                      sf::Style::Close, settings);
        window.setFramerateLimit(60);
    }
    void loadGameScene();
    void startEngine();
    void update(float elapsedTime);
    void draw();

    list<sf::Vector2f> findPath(sf::Vector2f src,sf::Vector2f dst);

    void fillCollisionMap();
};