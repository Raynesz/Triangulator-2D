#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include "util.h"

struct Line {
    int pointA;
    int pointB;
    sf::RectangleShape line;
};

struct Phase {
    std::string hint;
    std::string controls;
};

enum LineMode { ToPrevious, ToFirst };

enum PhaseIndex { One, OneHalf, Two, Three };

void initPhases(std::vector<Phase>&);
void createPoint(std::vector<sf::CircleShape>& points, float x, float y);
void createLine(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, LineMode lineMode);
void Triangulate();
void DetectTriangle();
bool IntersectingLinesExist(std::vector<sf::CircleShape>& points, std::vector<Line>& lines);
bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2);
