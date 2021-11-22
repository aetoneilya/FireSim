#include "Engine.h"

#include <iostream>

void Engine::startEngine() {
    humanText.loadFromFile("assets/maps/00-guide.png");
    fireText.loadFromFile("assets/maps/fire.png");

    loadGameScene();
    fillCollisionMap();

    // Fire test_fire({200, 50});
    humans.push_back({{150, 50}, humanText});
    humans.push_back({{165, 50}, humanText});
    humans.push_back({{150, 70}, humanText});

    sf::Clock clock;
    while (window.isOpen()) {
        const sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        update(elapsedTime.asSeconds());
        draw();
    }
}

void Engine::loadGameScene() {
    level.LoadFromFile("assets/maps/level.tmx");
    walls = level.GetAllObjects("block");
    exits = level.GetAllObjects("EXIT");
}

void Engine::update(float elapsedTime) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            fires.push_back({worldPos, fireText});
            int x = (int)worldPos.x / 16;
            int y = (int)worldPos.y / 16;
            collisionMap[y][x] = 2;
        }
    }
    sf::Vector2f movement = round(GetPlayerDirection() * cameraSpeed * elapsedTime);
    cameraPos += movement;
    camera.move(movement);

    for (std::list<Human>::iterator human = humans.begin(); human != humans.end(); ++human) {
        human->update(elapsedTime, collisionMap, !fires.empty(), exits);
        for (auto exit : exits) {
            if (human->rect.intersects(exit.rect))
                // human escape
                human = humans.erase(human);
        }
        for (std::list<Fire>::iterator fire = fires.begin(); fire != fires.end(); ++fire) {
            if (human->rect.intersects(fire->rect)) {
                // human->die
                human = humans.erase(human);
                break;
            }
        }
    }

    fireSpread();
    for (std::list<Fire>::iterator it = fires.begin(); it != fires.end(); ++it) {
        it->update(elapsedTime);
    }

    timer += elapsedTime;
}

void Engine::draw() {
    window.clear();
    window.setView(camera);

    sf::RenderWindow &target = window;
    level.Draw(target);

    for (auto human : humans) {
        human.draw(target);
    }

    for (auto fire : fires) {
        fire.draw(target);
    }

    window.display();
}

void Engine::fillCollisionMap() {
    int height = level.getHeight();
    int width = level.getWidth();

    collisionMap.resize(height);
    for (int i = 0; i < collisionMap.size(); i++) {
        collisionMap[i].resize(width);
    }

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            for (auto wall : walls) {
                collisionMap[i][j] += wall.rect.contains({j * 16.f + 1, i * 16.f + 1});
            }
        }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            std::cout << collisionMap[i][j];
        }
        std::cout << std::endl;
    }
}

// FIX FIX FIX CRASH AFTER FIRE SPREADS
void Engine::fireSpread() {
    if (timer > 3) {
        for (std::list<Fire>::iterator fire = fires.begin(); fire != fires.end(); ++fire) {
            int x = fire->getPosition().x / 16;
            int y = fire->getPosition().y / 16;

            if (rand() % 2) {  // fire spreads randomly
                int dx = rand() % 3 - 1;
                int dy = rand() % 3 - 1;

                if (x + dx > 0 && x + dx < collisionMap[0].size() && y + dy > 0 &&
                    y + dy < collisionMap.size() && collisionMap[y + dy][x + dx] == 0) {
                    fires.push_front({{(float)(x + dx) * 16, (float)(y + dy) * 16}, fireText});
                    collisionMap[y + dy][x + dx] = 2;
                }
            }
        }
        // std::cout << "Fires count = " << fires.size() << std::endl;
        timer = 0;
    }
}

sf::Vector2f Engine::GetPlayerDirection() {
    sf::Vector2f direction;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        direction.y = -1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        direction.y = +1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        direction.x = -1;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
               sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        direction.x = +1;
    }

    return normalize(direction);
}