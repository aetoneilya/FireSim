#include "Human.h"

#include "Fire.h"

void Human::draw(sf::RenderWindow &target) { anim.draw(target, position.x, position.y); }

void Human::startPanic(LevelData &levelData) {
    if (path.empty()) setPath(findShortestPath(levelData.exits));

    if (path.empty()) {
        // run around
        int x = rand() % 300 - 100;
        int y = rand() % 300 - 100;

        sf::Vector2f dest(position.x + x, position.y + y);
        setPath(findPath(position, dest));
        // Blocked destanation panic
    }
}

void Human::update(float elapsedTime, std::list<Fire> &fires, LevelData &levelData) {
    pathfind.setCollisionMap(levelData.collisionMap);

    // chill mode
    if (!levelData.isFire && path.empty()) {
        timer += elapsedTime;
        if (timer > waitTimer) {
            speed = 50.f;
            int x = rand() % 300 - 150;
            int y = rand() % 300 - 150;

            sf::Vector2f dest(position.x + x, position.y + y);

            setPath(findPath(position, dest));
            waitTimer = rand() % 5;
            timer = 0;
        }
    }

    // panic mode
    if (levelData.isFire && path.empty()) {
        speed = 100.f;
        panic = true;

        if (hero) {
            // fing extinguisher
            if (!hasExtingshr) {
                setPath(findShortestPathObj(levelData.objects, "extinguisher"));
            }
            // start fighting fire
            if (hasExtingshr) {
                std::vector<sf::Vector2f> positions = getFireFront(levelData.collisionMap);
                setPath(findShortestPathPos(positions));
            }

            if (path.empty()) {
                hero = false;
                hasExtingshr = false;
            }
        } else {
            startPanic(levelData);
        }
    }

    if (!pathClear(levelData.collisionMap)) {
        path.clear();
    }

    // take extinguisher
    if (hero && !hasExtingshr)
        for (std::list<Object>::iterator object = levelData.objects.begin();
             object != levelData.objects.end(); ++object) {
            if (rect.intersects(object->rect)) {
                hasExtingshr = true;
                levelData.objects.erase(object);
                break;
            }
        }

    // start walking
    if (abs(hypotf(position.x - aim.x, position.y - aim.y)) < 1) {
        if (!path.empty()) {
            aim = path.front();
            path.pop_front();
        } else {
            anim.set("STAY");
            rect.left = aim.x;
            rect.top = aim.y;
            position = aim;

            anim.tick(elapsedTime);
        }

        return;
    }

    sf::Vector2f movement = /*round*/ (normalize(aim - position) * speed * elapsedTime);

    // if hero and intersects fire anim set fight_fire
    if (hero && hasExtingshr) {
        sf::FloatRect rectRightExt;
        rectRightExt.height = 32;
        rectRightExt.width = 32;
        rectRightExt.left = position.x + 16;
        rectRightExt.top = position.y + 8;
        sf::FloatRect rectLeftExt(rectRightExt);
        rectLeftExt.left = position.x - rectLeftExt.width;

        for (std::list<Fire>::iterator fire = fires.begin(); fire != fires.end(); ++fire) {
            if (fire->rect.intersects(rectRightExt)) {
                fire->exting(1);
                anim.set("FIGHT_FIRE_RIGHT");
                movement = {0, 0};
            } else if (fire->rect.intersects(rectLeftExt)) {
                fire->exting(1);
                anim.set("FIGHT_FIRE_LEFT");
                movement = {0, 0};
            }
        }
    }

    if (abs(movement.x) >= abs(movement.y) && movement.x > 0) {
        anim.set("WALK_RIGHT");
    } else if (abs(movement.x) >= abs(movement.y) && movement.x < 0) {
        anim.set("WALK_LEFT");
    } else if (abs(movement.x) < abs(movement.y) && movement.y > 0) {
        anim.set("WALK_DOWN");
    } else if (abs(movement.x) < abs(movement.y) && movement.y < 0) {
        anim.set("WALK_UP");
    }

    rect.left += movement.x;
    rect.top += movement.y;
    position += movement;

    anim.tick(elapsedTime);
}

void Human::walkTo(const sf::Vector2f &new_aim) { aim = new_aim; }

void Human::setPath(std::list<sf::Vector2f> newPath) { path = newPath; }

sf::Vector2f Human::getPos() { return position; }

list<sf::Vector2f> Human::findPath(sf::Vector2f src, sf::Vector2f dst) {
    Pair from((int)src.x / 16, (int)src.y / 16);
    Pair to((int)dst.x / 16, (int)dst.y / 16);

    pathfind.aStarSearch(from, to);
    return pathfind.getPath();
}

bool Human::pathClear(std::vector<std::vector<int>> &collisionMap) {
    sf::Vector2f prevNode = path.front();
    for (auto node : path) {
        int x = (int)node.x / 16;
        int y = (int)node.y / 16;
        int prevX = (int)prevNode.x / 16;
        int prevY = (int)prevNode.y / 16;
        if (collisionMap[y][x] != 0 || collisionMap[y][prevX] != 0 || collisionMap[prevY][x]) return 0;
        prevNode = node;
    }
    return 1;
}

bool Human::isAlive() { return alive; }

bool Human::isPanic() { return panic; }

void Human::touchFire() {
    hp -= 1;
    if (hp < 0) {
        alive = false;
    }
}

list<sf::Vector2f> Human::findShortestPath(std::vector<TmxObject> objects) {
    sf::Vector2f nearest = {objects[0].rect.left, objects[0].rect.top};
    list<sf::Vector2f> minPath = findPath(position, nearest);
    int minPathLenght = minPath.size();

    for (auto object : objects) {
        sf::Vector2f nearest = {object.rect.left, object.rect.top};
        list<sf::Vector2f> newPath = findPath(position, nearest);
        int newPathLenght = newPath.size();

        if (newPathLenght < minPathLenght && newPathLenght != 0) {
            minPath = newPath;
            minPathLenght = newPathLenght;
        }
    }

    return minPath;
}

list<sf::Vector2f> Human::findShortestPathPos(std::vector<sf::Vector2f> positions) {
    sf::Vector2f nearest = positions[0];
    list<sf::Vector2f> minPath = findPath(position, nearest);
    int minPathLenght = minPath.size();

    for (auto object : positions) {
        sf::Vector2f nearest = object;
        list<sf::Vector2f> newPath = findPath(position, nearest);
        int newPathLenght = newPath.size();

        if (newPathLenght < minPathLenght && newPathLenght != 0) {
            minPath = newPath;
            minPathLenght = newPathLenght;
        }
    }

    return minPath;
}

list<sf::Vector2f> Human::findShortestPathObj(std::list<Object> objects, string type) {
    sf::Vector2f nearest = {objects.front().rect.left, objects.front().rect.top};
    list<sf::Vector2f> minPath = findPath(position, nearest);
    int minPathLenght = minPath.size();

    for (auto object : objects) {
        if (object.getType() != type) {
            continue;
        }
        sf::Vector2f nearest = {object.rect.left, object.rect.top};
        list<sf::Vector2f> newPath = findPath(position, nearest);
        int newPathLenght = newPath.size();

        if (newPathLenght < minPathLenght && newPathLenght != 0) {
            minPath = newPath;
            minPathLenght = newPathLenght;
        }
    }

    return minPath;
}

int Human::getHP() { return hp; }

std::vector<sf::Vector2f> Human::getFireFront(std::vector<std::vector<int>> collisionMap) {
    std::vector<sf::Vector2f> fireFront;
    fireFront.clear();
    int max_i = collisionMap.size();
    int max_j = collisionMap[0].size();
    for (int i = 0; i < max_i; i++)
        for (int j = 0; j < max_j; j++) {
            if (collisionMap[i][j] == 2 && j - 2 > 0 && collisionMap[i][j - 2] == 0) {
                fireFront.push_back({(float)(j - 1) * 16, (float)i * 16});
            }
            if (collisionMap[i][j] == 2 && j + 2 < max_j && collisionMap[i][j + 2] == 0) {
                fireFront.push_back({(float)(j + 1) * 16, (float)i * 16});
            }
        }

    return fireFront;
}