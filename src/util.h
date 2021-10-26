#pragma once

#define PI 3.14159265359

inline double calculateDistance(sf::Vector2f a, sf::Vector2f b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

inline sf::Vector2f calculateMidpoint(sf::Vector2f a, sf::Vector2f b) {
    return sf::Vector2f((b.x + a.x) / 2, (b.y + a.y) / 2);
}

inline double calculateLineAngle(sf::Vector2f a, sf::Vector2f b) {
    return atan2(a.y - b.y, a.x - b.x) * 180.0 / PI;
}
