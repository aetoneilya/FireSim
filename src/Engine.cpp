#include <iostream>

#include "Engine.h"

void Engine::startEngine()
{
    loadGameScene();
    fillCollisionMap();
    pathfind.setCollisionMap(collisionMap);

    Human test_human({150, 50});
    humans.push_back(test_human);

    sf::Clock clock;
    while (window.isOpen())
    {
        const sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        update(elapsedTime.asSeconds());
        draw();
    }
}

void Engine::loadGameScene()
{
    level.LoadFromFile("assets/maps/level.tmx");
    walls = level.GetAllObjects("block");
}

void Engine::update(float elapsedTime)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            // std::cout << (float)event.mouseButton.x << " " << (float)event.mouseButton.y << std::endl;
            // humans[0].walkTo({(float)event.mouseButton.x, (float)event.mouseButton.y});
            humans[0].setPath(findPath(humans[0].getPos(), {(float)event.mouseButton.x, (float)event.mouseButton.y}));
        }
    }

    for (int i = 0; i < humans.size(); i++)
    {
        humans[i].update(elapsedTime, collisionMap);
    }
}

void Engine::draw()
{
    window.clear();

    sf::RenderWindow &target = window;
    level.Draw(target);

    for (auto human : humans)
    {
        human.draw(target);
    }

    window.display();
}

void Engine::fillCollisionMap()
{
    int height = level.getHeight();
    int width = level.getWidth();

    collisionMap.resize(height);
    for (int i = 0; i < collisionMap.size(); i++)
    {
        collisionMap[i].resize(width);
    }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            for (auto wall : walls)
            {
                collisionMap[i][j] += wall.rect.contains({j * 16.f + 1, i * 16.f + 1});
            }
        }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            std::cout << collisionMap[i][j];
        }
        std::cout << std::endl;
    }
}

list<sf::Vector2f> Engine::findPath(sf::Vector2f src,sf::Vector2f dst){
    Pair from((int)src.x / 16, (int)src.y / 16);
    Pair to((int)dst.x / 16, (int)dst.y / 16);

    std::cout << from.first << " " << from.second << std::endl;
    pathfind.aStarSearch(from, to);
    return pathfind.getPath();
}