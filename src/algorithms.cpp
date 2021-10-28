#include "main.h"
/*
for (int i = 0; i < temps.size(); i++) {
    std::cout << "" << std::endl;
}
*/
void Triangulate(std::vector<sf::CircleShape>& points, std::vector<Line>& lines) {
    SortPoints(points, lines);
    
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < i; j++) {
            bool keep = true;
            Line temp{ i , j, false};
            for (auto line : lines) {
                if (LinesAreEqual(line, temp)) {
                    keep = false;
                    break;
                }
                if (LinesShareAPoint(line, temp)) continue;
                if (doIntersect(points[line.pointA].getPosition(), points[line.pointB].getPosition(),
                    points[temp.pointA].getPosition(), points[temp.pointB].getPosition())) {
                    keep = false;
                    break;
                }
            }
            if (LineLiesOutside(points, lines, temp)) keep = false;
            if (keep) lines.push_back(createLine(points, temp));
        }
    }
}

bool LineLiesOutside(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, Line line) {
    int count = 0;
    sf::Vector2f midPoint = calculateMidpoint(points[line.pointA].getPosition(), points[line.pointB].getPosition());
    for (auto lineIter : lines) {
        if (lineIter.hull && doIntersect(sf::Vector2f{0, midPoint.y}, midPoint, points[lineIter.pointA].getPosition(), points[lineIter.pointB].getPosition())) count++;
    }
    if (count % 2 == 0) return true;
    else return false;
}

bool LinesShareAPoint(Line a, Line b) {
    if (a.pointA == b.pointA || a.pointA == b.pointB) return true;
    if (a.pointB == b.pointA || a.pointB == b.pointB) return true;
    return false;
}

void DetectTriangle() {

}

bool LinesAreEqual(Line a, Line b) {
    if ((a.pointA == b.pointA && a.pointB == b.pointB) ||
        (a.pointA == b.pointB && a.pointB == b.pointA)) return true;
    else return false;
}

void SortPoints(std::vector<sf::CircleShape>& points, std::vector<Line>& lines) {
    std::vector<SortTemp> temps = {};
    for (int i = 0; i < points.size(); i++) {
        temps.push_back(SortTemp{i, points[i].getPosition().x});
    }

    sort(temps.begin(), temps.end(), [](const auto& lhs, const auto& rhs) { return lhs.x < rhs.x; });
    sort(points.begin(), points.end(), [](const auto& lhs, const auto& rhs) { return lhs.getPosition().x < rhs.getPosition().x; });

    std::vector<Line> newLines = {};

    for (auto line : lines) {
        bool aChanged = false;
        bool bChanged = false;
        for (int i = 0; i < temps.size(); i++) {
            if (!aChanged && line.pointA == temps[i].index) {
                line.pointA = i;
                aChanged = true;
            }
            if (!bChanged && line.pointB == temps[i].index) {
                line.pointB = i;
                bChanged = true;
            }
        }
        newLines.push_back(createLine(points, line));
    }

    lines.clear();
    lines = newLines;
}

bool IntersectingLinesExist(std::vector<sf::CircleShape>& points, std::vector<Line>& lines) {
    for (int i = 0; i < lines.size(); i++) {
        for (int j = i; j < lines.size(); j++) {
            if (i == 0 && j == (lines.size() - 1)) continue;
            if (i == j || j == i - 1 || j == i + 1) continue;
            if (doIntersect(points[lines[i].pointA].getPosition(), points[lines[i].pointB].getPosition(), points[lines[j].pointA].getPosition(), points[lines[j].pointB].getPosition())) {
                return true;
            }
        }
    }
    return false;
}

// Functions that help detect if two line segments intersect (taken by geeksforgeeks.org)
bool onSegment(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

int orientation(sf::Vector2f p, sf::Vector2f q, sf::Vector2f r)
{
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // collinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool doIntersect(sf::Vector2f p1, sf::Vector2f q1, sf::Vector2f p2, sf::Vector2f q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are collinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1)) return true;

    // p1, q1 and q2 are collinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1)) return true;

    // p2, q2 and p1 are collinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2)) return true;

    // p2, q2 and q1 are collinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2)) return true;

    return false; // Doesn't fall in any of the above cases
}
