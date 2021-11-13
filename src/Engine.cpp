#include <iostream>

#include "Engine.h"

void Engine::startEngine()
{

    humanText.loadFromFile("assets/maps/00-guide.png");
    fireText.loadFromFile("assets/maps/fire.png");

    loadGameScene();
    fillCollisionMap();
    pathfind.setCollisionMap(collisionMap);

    // Fire test_fire({200, 50});
    fires.push_back({{170, 50}, fireText});
    humans.push_back({{150, 50}, humanText});

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
            humans[0].setPath(findPath(humans[0].getPos(), {(float)event.mouseButton.x, (float)event.mouseButton.y}));
        }
    }

    for (int i = 0; i < humans.size(); i++)
    {
        humans[i].update(elapsedTime, collisionMap);
    }
    int fsize = fires.size();
    for (int i = 0; i < fsize; i++)
    {
        fires[i].update(elapsedTime);

        int y = fires[i].getPosition().y / 16 + rand() % 2;
        int x = fires[i].getPosition().x / 16 + rand() % 2;
        if (collisionMap[y][x] == 0)
        {
            collisionMap[y][x] == 2;
            fires.push_back({{(float)16 * x, (float)16 * y}, fireText});
        }
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

    for (auto fire : fires)
    {
        fire.draw(target);
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

list<sf::Vector2f> Engine::findPath(sf::Vector2f src, sf::Vector2f dst)
{
    Pair from((int)src.x / 16, (int)src.y / 16);
    Pair to((int)dst.x / 16, (int)dst.y / 16);

    std::cout << from.first << " " << from.second << std::endl;
    pathfind.aStarSearch(from, to);
    return pathfind.getPath();
}