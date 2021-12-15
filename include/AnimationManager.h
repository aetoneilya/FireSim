#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <map>
#include <string>
#include <utility>

class Animation {
   public:
    std::vector<sf::IntRect> frames;
    float currentFrame, speed;
    bool isPlaying;
    sf::Sprite sprite;

    Animation(sf::Texture &t, int x, int y, int w, int h, int count, float Speed, int step);
    Animation();
    void tick(float time);
};

class AnimationManager {
   private:
    std::string currentAnim;
    std::map<std::string, Animation> animList;

   public:
    void create(std::string name, sf::Texture &t, int x, int y, int w, int h, int count, float speed,
                int step);

    void draw(sf::RenderWindow &window, int, int);

    void set(std::string name);

    void tick(float time);

    void pause();

    void play();
};