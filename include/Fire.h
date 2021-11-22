#include "AnimationManager.h"

class Fire
{
private:
    sf::Vector2f position;
    sf::Texture q;
    const float fireRate = 1.f;

public:
    sf::FloatRect rect;
    AnimationManager anim;

    Fire(const sf::Vector2f &Position, sf::Texture &fireText)
    {
        // fix this need arguments
        anim.create("FIRE", fireText, 0, 0, 16, 16, 4, 5, 16);
        anim.set("FIRE");
        anim.play();

        position.x = 16 * ((int)Position.x / 16);
        position.y = 16 * ((int)Position.y / 16);

        rect.left = position.x;
        rect.top = position.y;
        rect.width = 16;
        rect.height = 16;

    }
    void draw(sf::RenderWindow &target)
    {
        anim.draw(target, position.x, position.y);
    }
    void update(float elapsedTime)
    {
        anim.tick(elapsedTime);
    }

    sf::Vector2f getPosition(){
        return position;
    }
};