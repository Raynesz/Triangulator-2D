#include "SFML/Graphics.hpp"
#include <iostream>

#define PI 3.14159265359

struct Line {
    int pointA;
    int pointB;
    sf::RectangleShape line;
};

enum LineMode {ToPrevious, ToFirst};

enum class Phase {One=1, Two};

void createPoint(std::vector<sf::CircleShape>& points, float x, float y);
void createLine(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, LineMode lineMode);
float calculateDistance(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f calculateMidpoint(sf::Vector2f a, sf::Vector2f b);
double calculateLineAngle(sf::Vector2f a, sf::Vector2f b);

int main()
{
    float width = sf::VideoMode::getDesktopMode().width;
    float height = sf::VideoMode::getDesktopMode().height;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(width, height), "Triangulator-2D", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);

    std::vector<sf::CircleShape> points = {};
    std::vector<Line> lines = {};
    
    sf::Font font;
    if (!font.loadFromFile("RussoOne-Regular.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setString("Triangulator-2D");
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);

    sf::Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(24);
    debugText.setFillColor(sf::Color::White);
    debugText.setOrigin(0, 24);
    debugText.setPosition(5, height-5);

    Phase phase = Phase::One;

    sf::Text phaseText;
    phaseText.setFont(font);
    phaseText.setCharacterSize(24);
    phaseText.setFillColor(sf::Color::White);

    while (window.isOpen())
    {
        // FRAME TIMER
        sf::Clock clock;
        float dt = 0.f;

        // INPUT
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
    
            if (event.type == sf::Event::MouseButtonReleased)
            {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (phase == Phase::One) {
                        sf::Vector2i position = sf::Mouse::getPosition();
                        createPoint(points, position.x, position.y);
                        if (points.size() > 1) createLine(points, lines, ToPrevious);
                    }
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                if (phase == Phase::One) {
                    phase = Phase::Two;
                    createLine(points, lines, ToFirst);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
            {
                phase = Phase::One;
                points.clear();
                lines.clear();
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                std::cout << "Points: " << points.size() << "  |  Lines: " << lines.size() << std::endl;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        // UPDATE
        switch (phase) {
        case Phase::One:
            phaseText.setString("Phase: 1");
            break;
        case Phase::Two:
            phaseText.setString("Phase: 2");
            break;
        default:
            break;
        }
        phaseText.setOrigin(phaseText.getLocalBounds().width/2, phaseText.getLocalBounds().height / 2);
        phaseText.setPosition(width / 2, 5);
        debugText.setString("Points: " + std::to_string(points.size()) + "  |  Lines: " + std::to_string(lines.size()));
        
        // RENDER
        window.clear(sf::Color(18, 33, 43)); // Color background
        for (int i = 0; i < lines.size(); i++) window.draw(lines[i].line);
        for (int i = 0; i < points.size(); i++) window.draw(points[i]);
        window.draw(fpsText);
        window.draw(phaseText);
        window.draw(debugText);
        window.display();

        //FPS COUNTER
        dt = clock.restart().asSeconds();
        fpsText.setString("FPS: "+std::to_string((int)(1.0f/dt)));
    }

    return 0;
}

void createPoint(std::vector<sf::CircleShape>& points, float x, float y) {
    sf::CircleShape point(7.0f);
    point.setOrigin(7.0f, 7.0f);
    point.setFillColor(sf::Color(255, 255, 0)); // Color point
    point.setPosition(x, y);
    points.push_back(point);
}

void createLine(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, LineMode lineMode) {
    if (points.size() > 1) {
        Line line = Line();
        line.pointA = (lineMode == ToPrevious) ? points.size() - 2 : 0;
        line.pointB = points.size() - 1;

        float distance = calculateDistance(points[line.pointA].getPosition(), points[line.pointB].getPosition());
        sf::Vector2f position = calculateMidpoint(points[line.pointA].getPosition(), points[line.pointB].getPosition());
        double angle = calculateLineAngle(points[line.pointA].getPosition(), points[line.pointB].getPosition());
        sf::RectangleShape rectangle(sf::Vector2f(distance, 5.f));
        rectangle.setOrigin(distance / 2.0f, 2.5f);
        rectangle.setPosition(position);
        rectangle.rotate(angle);
        rectangle.setFillColor(sf::Color::Red);
        line.line = rectangle;
        lines.push_back(line);
    }
}

float calculateDistance(sf::Vector2f a, sf::Vector2f b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

sf::Vector2f calculateMidpoint(sf::Vector2f a, sf::Vector2f b) {
    return sf::Vector2f((b.x + a.x)/2, (b.y + a.y) / 2);
}

double calculateLineAngle(sf::Vector2f a, sf::Vector2f b) {
    return atan2(a.y - b.y, a.x - b.x) * 180.0 / PI;
}
