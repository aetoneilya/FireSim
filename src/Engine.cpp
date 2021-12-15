#include "Engine.h"

#include <iostream>

void Engine::startEngine() {
    try {
        if (!humanTxtr.loadFromFile("../assets/human.png")) throw Exception("can't load texture human.png");
        if (!fireTxtr.loadFromFile("../assets/fire.png")) throw Exception("can't load texture fire.png");
        if (!extingTxtr.loadFromFile("../assets/extinguisher.png"))
            throw Exception("can't loat texture extinguisher.png");
        if (!deadTxtr.loadFromFile("../assets/dead.png")) throw Exception("can't load texture dead.png");
        if (!postFireTxtr.loadFromFile("../assets/postFire.png")) throw Exception("can't load texture postFire.png");

        if (!deathSoundBuf.loadFromFile("../assets/mixkit-explainer-video-game-alert-sweep-236.wav"))
            throw Exception("can't load sound mixkit-explainer-video-game-alert-sweep-236.wav");
        if (!safeSoundBuf.loadFromFile("../assets/mixkit-game-ball-tap-2073.wav"))
            throw Exception("can't load sound mixkit-game-ball-tap-2073.wav");
        if (!extinguisherSoundBuf.loadFromFile("../assets/exitnguisher.wav"))
            throw Exception("can't load sound exitnguisher.wav");
        if (!startFireSoundBuf.loadFromFile("../assets/start_fire.wav"))
            throw Exception("can't load sound start_fire.wav");
    } catch (Exception& exception) {
        std::cerr << "ERROR: (" << exception.what() << ")\n";
        return;
    }

    deathSound.setBuffer(deathSoundBuf);
    safeSound.setBuffer(safeSoundBuf);
    extinguisherSound.setBuffer(extinguisherSoundBuf);
    startFireSound.setBuffer(startFireSoundBuf);

    try {
        loadGameScene();
        fillCollisionMap();

    } catch (Exception& exception) {
        std::cerr << "ERROR: (" << exception.what() << ")\n";
        return;
    }
    
    sf::Clock clock;
    while (window.isOpen()) {
        const sf::Time elapsedTime = clock.getElapsedTime();
        clock.restart();

        update(elapsedTime.asSeconds());
        draw();
    }
}

void Engine::loadGameScene() {
    if (!level.LoadFromFile("../assets/maps/level.tmx")) throw Exception("can't load level data");

    levelData.walls = level.GetAllObjects("block");
    levelData.exits = level.GetAllObjects("EXIT");
    std::vector<TmxObject> extingTmx = level.GetAllObjects("extinguisher");
    for (auto exting : extingTmx) {
        levelData.objects.push_back({"extinguisher", extingTxtr, exting.rect});
    }
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
            addFire(worldPos);
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L) {
            camera.zoom(2);
            zoom *= 2;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::K) {
            camera.zoom(0.5);
            zoom /= 2;
        }
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
            fires.clear();
            humans.clear();
            levelData.visuals.clear();
            levelData.objects.clear();
            humanCount = 0;
            safeCount = 0;
            deadCount = 0;
            loadGameScene();
        }
    }

    sf::Vector2f movement = round(GetPlayerDirection() * cameraSpeed * elapsedTime * zoom);
    cameraPos += movement;
    camera.move(movement);

    for (auto exit : levelData.exits) {
        levelData.humanTimer += elapsedTime;
        if (humanCount < humanMax && levelData.humanTimer > waitHumanSpawn) {
            humans.push_back({{exit.rect.left + 5, exit.rect.top + 5}, humanTxtr});
            humanCount++;
            waitHumanSpawn = rand() % 5;
            levelData.humanTimer = 0;
        }
    }

    levelData.isFire = !fires.empty();
    for (std::list<Human>::iterator human = humans.begin(); human != humans.end(); ++human) {
        human->update(elapsedTime, fires, levelData);
        for (std::list<Fire>::iterator fire = fires.begin(); fire != fires.end(); ++fire) {
            if (human->rect.intersects(fire->rect)) {
                human->touchFire();
                break;
            }
        }
        if (human->isAlive() == false) {
            levelData.visuals.push_back({"dead", deadTxtr, human->rect});
            human = humans.erase(human);
            deadCount++;
            deathSound.play();
        } else if (human->isPanic() && isIntersects(human->rect, levelData.exits)) {
            human = humans.erase(human);
            safeCount++;
            safeSound.play();
        }
    }

    fireSpread();
    for (std::list<Fire>::iterator it = fires.begin(); it != fires.end(); ++it) {
        it->update(elapsedTime);
        if (it->getHP() < 0) {
            levelData.visuals.push_back({"postFire", postFireTxtr, it->rect});
            levelData.collisionMap[it->getPosition().y / 16][it->getPosition().x / 16] = 0;
            it = fires.erase(it);
            extinguisherSound.play();
        }
    }

    levelData.fireTimer += elapsedTime;
}

void Engine::draw() {
    window.clear();
    window.setView(camera);
    sf::RenderWindow& target = window;
    level.Draw(target);

    for (auto visual : levelData.visuals) target.draw(visual.getSprite());

    for (auto fire : fires) fire.draw(target);

    for (auto object : levelData.objects) target.draw(object.getSprite());

    int i = 0;
    for (auto human : humans) human.draw(target);

    for (auto human : humans) {
        sf::RectangleShape hpBar(
            {(float)0.1 * camera.getSize().x * human.getHP() / 100, camera.getSize().y / 60});
        sf::Uint8 green = 250 * (float)human.getHP() / 100.f;
        sf::Uint8 red = 250 - green;

        hpBar.setFillColor({red, green, 0});
        float x = camera.getCenter().x - camera.getSize().x / 2 + camera.getSize().y / 40;
        float y = camera.getCenter().y - camera.getSize().y / 2 + i * camera.getSize().y / 40 +
                  camera.getSize().y / 40;
        hpBar.setPosition(x, y);
        target.draw(hpBar);
        i++;
    }

    string =
        "Humans in building: " + to_string(humanCount - safeCount - deadCount) + "/" + to_string(humanMax);
    string += "\nEvacuated: " + to_string(safeCount) + "\nDied: " + to_string(deadCount);

    text.setPosition(camera.getCenter().x + 150, camera.getCenter().y - 300);
    text.setString(string);
    window.draw(text);

    window.display();
}

void Engine::fillCollisionMap() {
    int height = level.getHeight();
    int width = level.getWidth();

    levelData.collisionMap.resize(height);
    for (int i = 0; i < levelData.collisionMap.size(); i++) levelData.collisionMap[i].resize(width);

    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++) {
            levelData.collisionMap[i][j] = 0;
            for (auto wall : levelData.walls) {
                if (wall.rect.contains({j * 16.f + 1, i * 16.f + 1})) {
                    levelData.collisionMap[i][j] = 1;
                    break;
                }
            }
        }
}

void Engine::fireSpread() {
    if (levelData.fireTimer > 3) {
        for (std::list<Fire>::iterator fire = fires.begin(); fire != fires.end(); ++fire) {
            int x = fire->getPosition().x / 16;
            int y = fire->getPosition().y / 16;

            if (rand() % 2) {  // fire spreads randomly
                int dx = rand() % 3 - 1;
                int dy = rand() % 3 - 1;

                if (x + dx > 0 && x + dx < levelData.collisionMap[0].size() && y + dy > 0 &&
                    y + dy < levelData.collisionMap.size() && levelData.collisionMap[y + dy][x + dx] == 0) {
                    fires.push_front({{(float)(x + dx) * 16, (float)(y + dy) * 16}, fireTxtr});
                    levelData.collisionMap[y + dy][x + dx] = 2;
                }
            }
        }
        levelData.fireTimer = 0;
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

bool Engine::isIntersects(sf::FloatRect& rect, std::vector<TmxObject>& objects) {
    for (auto object : objects) {
        if (rect.intersects(object.rect)) {
            return true;
        }
    }
    return false;
}

void Engine::addFire(sf::Vector2f worldPos) {
    int x = (int)worldPos.x / 16;
    int y = (int)worldPos.y / 16;
    if (x < 0 || x > levelData.collisionMap[0].size() || y < 0 || y > levelData.collisionMap.size()) {
        return;
    }
    if (levelData.collisionMap[y][x] == 0) {
        fires.push_back({worldPos, fireTxtr});
        levelData.collisionMap[y][x] = 2;
        startFireSound.play();
    }
}