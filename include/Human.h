#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

#include "AnimationManager.h"
#include "Astar.h"
#include "TmxLevel.h"
#include "VectorFlow.h"

enum directions { DOWN, LEFT, RIGHT, UP };

class Human {
   private:
    bool panic;
    float speed;

    directions dir;
    sf::Vector2f position;
    sf::Vector2f aim;

    std::list<sf::Vector2f> path;
    sf::Texture q;

    Astar pathfind;

    float timer;
    int waitTimer;

    list<sf::Vector2f> findPath(sf::Vector2f src, sf::Vector2f dst);

    bool pathClear(std::vector<std::vector<int>> &collisionMap);

   public:
    sf::FloatRect rect;
    AnimationManager anim;

    Human(const sf::Vector2f &Position, sf::Texture &humanText) {
        speed = 50.f;
        position = Position;
        aim = Position;

        rect.left = position.x;
        rect.top = position.y;
        rect.width = 16;
        rect.height = 16;

        panic = false;
        dir = DOWN;
        waitTimer = 0;

        anim.create("STAY", humanText, 16, 0, 16, 16, 1, 1, 16);
        anim.create("WALK_DOWN", humanText, 0, 0, 16, 16, 3, 5, 16);
        anim.create("WALK_LEFT", humanText, 0, 16, 16, 16, 3, 5, 16);
        anim.create("WALK_RIGHT", humanText, 0, 32, 16, 16, 3, 5, 16);
        anim.create("WALK_UP", humanText, 0, 48, 16, 16, 3, 5, 16);
        // TODO : add dying animation
    }

    void draw(sf::RenderWindow &target);

    void walkTo(const sf::Vector2f &movement);

    void MoveBy(const sf::Vector2f &movement);
    void MoveTo(const sf::Vector2f &position);

    void chill();

    void update(float elapsedTime, std::vector<std::vector<int>> &collisionMap, bool isFire,
                std::vector<TmxObject> &exits);

    void setPath(std::list<sf::Vector2f> newPath);

    sf::Vector2f getPos();
};
