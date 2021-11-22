#include "VectorFlow.h"

sf::Vector2f round(const sf::Vector2f value) { return sf::Vector2f(roundf(value.x), roundf(value.y)); }

sf::Vector2f normalize(const sf::Vector2f value) {
    const float length = std::hypotf(value.x, value.y);
    if (length < FLT_EPSILON) {
        return sf::Vector2f(0, 0);
    }
    return value / length;
}