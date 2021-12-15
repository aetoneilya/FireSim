#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <utility>

#include "AnimationManager.h"
#include "Astar.h"
#include "Fire.h"
#include "LevelData.h"
#include "VectorFlow.h"

enum directions { DOWN, LEFT, RIGHT, UP };

class Human {
   private:
    int hp;
    bool alive;
    bool panic;
    bool hero;
    bool hasExtingshr;
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
    list<sf::Vector2f> findShortestPath(std::vector<TmxObject> objects);
    list<sf::Vector2f> findShortestPathObj(std::list<Object> objects, string type);
    list<sf::Vector2f> findShortestPathPos(std::vector<sf::Vector2f> positions);

    std::vector<sf::Vector2f> getFireFront(std::vector<std::vector<int>> collisionMap);

    bool pathClear(std::vector<std::vector<int>> &collisionMap);

   public:
    sf::FloatRect rect;
    AnimationManager anim;

    Human(const sf::Vector2f &Position, sf::Texture &humanTxtr) {
        speed = 50.f;
        position = Position;
        aim = Position;

        rect.left = position.x;
        rect.top = position.y;
        rect.width = 16;
        rect.height = 16;

        hp = 100;
        alive = true;
        panic = false;

        hero = ((rand() % 10) == 1) ? true : false;
        hasExtingshr = false;

        dir = DOWN;
        waitTimer = 0;

        anim.create("STAY", humanTxtr, 16, 0, 16, 16, 1, 1, 16);
        anim.create("WALK_DOWN", humanTxtr, 0, 0, 16, 16, 3, 5, 16);
        anim.create("WALK_LEFT", humanTxtr, 0, 16, 16, 16, 3, 5, 16);
        anim.create("WALK_RIGHT", humanTxtr, 0, 32, 16, 16, 3, 5, 16);
        anim.create("WALK_UP", humanTxtr, 0, 48, 16, 16, 3, 5, 16);
        anim.create("FIGHT_FIRE_RIGHT", humanTxtr, 0, 64, 16, 16, 3, 5, 16);
        anim.create("FIGHT_FIRE_DOWN", humanTxtr, 0, 80, 16, 16, 3, 5, 16);
        anim.create("FIGHT_FIRE_LEFT", humanTxtr, 0, 96, 16, 16, 3, 5, 16);
        anim.create("FIGHT_FIRE_UP", humanTxtr, 0, 112, 16, 16, 3, 5, 16);

        anim.tick(0);
    }

    void draw(sf::RenderWindow &target);

    void startPanic(LevelData &levelData);

    void walkTo(const sf::Vector2f &movement);

    void MoveBy(const sf::Vector2f &movement);
    void MoveTo(const sf::Vector2f &position);

    void chill();

    void update(float elapsedTime, std::list<Fire> &fires, LevelData &levelData);

    void setPath(std::list<sf::Vector2f> newPath);

    sf::Vector2f getPos();
    int getHP();
    bool isAlive();
    bool isPanic();
    void touchFire();
};
