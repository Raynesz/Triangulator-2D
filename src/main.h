#pragma once

#include "SFML/Graphics.hpp"
#include <iostream>
#include "util.h"

struct Line {
    int pointA;
    int pointB;
    bool hull;
    sf::RectangleShape line;
};

struct Phase {
    std::string hint;
    std::string controls;
};

struct SortTemp {
    int index;
    float x;
};

enum LineMode { ToPrevious, ToFirst };

enum PhaseIndex { One, OneHalf, Two, Three };

void initPhases(std::vector<Phase>&);
void createPoint(std::vector<sf::CircleShape>& points, float x, float y);
void createLine(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, LineMode lineMode);
Line createLine(std::vector<sf::CircleShape>& points, Line newLine);
void Triangulate(std::vector<sf::CircleShape>& points, std::vector<Line>& lines);
void SortPoints(std::vector<sf::CircleShape>& points, std::vector<Line>& lines);
bool LineLiesOutside(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, Line line);
bool LinesShareAPoint(Line a, Line b);
bool LinesAreEqual(Line a, Line b);
void DetectTriangle();
bool IntersectingLinesExist(std::vector<sf::CircleShape>& points, std::vector<Line>& lines);
bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r);
bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2);
