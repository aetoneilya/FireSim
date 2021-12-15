#pragma once

#include "TmxLevel.h"

class Object {
   private:
    string type;
    sf::Vector2f position;
    sf::Sprite sprite;

   public:
    sf::FloatRect rect;

    sf::Vector2f getPos() { return position; }

    Object(string objectType, sf::Texture& objTxtr, sf::FloatRect objRect) {
        type = objectType;
        rect = objRect;
        position = {rect.left, rect.top};
        sprite.setTexture(objTxtr);
        sprite.setPosition(position);
    };
    sf::Sprite getSprite() { return sprite; }
    string getType() { return type; }
};

struct LevelData {
    std::vector<std::vector<int>> collisionMap;
    std::vector<TmxObject> walls;
    std::vector<TmxObject> exits;
    std::list<Object> objects;
    std::list<Object> visuals;
    float fireTimer;
    float humanTimer;
    bool isFire;
};