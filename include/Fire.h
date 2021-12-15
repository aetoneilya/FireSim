#pragma once
#include "AnimationManager.h"

class Fire {
   private:
    int hp;
    sf::Vector2f position;
    sf::Texture q;
    const float fireRate = 1.f;

   public:
    sf::FloatRect rect;
    AnimationManager anim;

    Fire(const sf::Vector2f &Position, sf::Texture &fireTxtr) {
        // fix this need arguments
        anim.create("FIRE", fireTxtr, 0, 0, 16, 16, 4, 5, 16);
        anim.set("FIRE");
        anim.play();

        position.x = 16 * ((int)Position.x / 16);
        position.y = 16 * ((int)Position.y / 16);

        rect.left = position.x;
        rect.top = position.y;
        rect.width = 16;
        rect.height = 16;

        hp = 30;
    }

    void exting(int minusHP) { hp -= minusHP; }

    void draw(sf::RenderWindow &target) { anim.draw(target, position.x, position.y); }
    void update(float elapsedTime) { anim.tick(elapsedTime); }

    sf::Vector2f getPosition() { return position; }
    int getHP() { return hp; }
};