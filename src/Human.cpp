#include "Human.h"

void Human::draw(sf::RenderWindow &target)
{
    anim.draw(target, position.x, position.y);
}

void Human::update(float elapsedTime, std::vector<std::vector<int>> &collisionMap)
{

    if (abs(hypotf(position.x - aim.x, position.y - aim.y)) < 1)
    {
        anim.set("STAY");
        rect.left = aim.x;
        rect.top = aim.y;
        position = aim;

        anim.tick(elapsedTime);

        if (!path.empty())
        {
            aim = path.front();
            path.pop_front();
        }

        return;
    }

    sf::Vector2f movement = round(normalize(aim - position) * speed * elapsedTime);

    if (abs(movement.x) >= abs(movement.y) && movement.x > 0)
    {
        anim.set("WALK_RIGHT");
    }
    else if (abs(movement.x) >= abs(movement.y) && movement.x < 0)
    {
        anim.set("WALK_LEFT");
    }
    else if (abs(movement.x) < abs(movement.y) && movement.y > 0)
    {
        anim.set("WALK_DOWN");
    }
    else if (abs(movement.x) < abs(movement.y) && movement.y < 0)
    {
        anim.set("WALK_UP");
    }

    rect.left += movement.x;
    rect.top += movement.y;
    position += movement;

    anim.tick(elapsedTime);
}

void Human::walkTo(const sf::Vector2f &new_aim)
{
    aim = new_aim;
}

sf::Vector2f Human::round(const sf::Vector2f value)
{
    return sf::Vector2f(roundf(value.x), roundf(value.y));
}

sf::Vector2f Human::normalize(const sf::Vector2f value)
{
    const float length = std::hypotf(value.x, value.y);
    if (length < FLT_EPSILON)
    {
        return sf::Vector2f(0, 0);
    }
    return value / length;
}

void Human::setPath(std::list<sf::Vector2f> newPath)
{
    path = newPath;
}

sf::Vector2f Human::getPos()
{
    return position;
}