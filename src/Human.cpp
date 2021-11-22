#include "Human.h"

void Human::draw(sf::RenderWindow &target) { anim.draw(target, position.x, position.y); }

void Human::update(float elapsedTime, std::vector<std::vector<int>> &collisionMap, bool isFire,
                   std::vector<TmxObject> &exits) {
    pathfind.setCollisionMap(collisionMap);

    // chill mode
    if (!isFire && path.empty()) {
        timer += elapsedTime;
        if (timer > waitTimer) {
            speed = 50.f;
            int x = rand() % 300 - 100;
            int y = rand() % 300 - 100;

            sf::Vector2f dest(position.x + x, position.y + y);

            std::cout << "Walking around" << std::endl;
            setPath(findPath(position, dest));
            waitTimer = rand() % 5;
            timer = 0;
        }
    }

    // panic mode
    if (isFire && path.empty()) {
        speed = 100.f;
        std::cout << "Start panic\n";
        sf::Vector2f dest(exits[0].rect.left, exits[0].rect.top);

        setPath(findPath(position, dest));
        if (path.empty()) {
            // run around
            int x = rand() % 300 - 100;
            int y = rand() % 300 - 100;

            sf::Vector2f dest(position.x + x, position.y + y);
            setPath(findPath(position, dest));
            std::cout << "Blocked destanation panic" << std::endl;
        }
    }

    if (!pathClear(collisionMap)) {
        std::cout << "Path blocked" << std::endl;
        path.clear();
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