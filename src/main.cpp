#include "SFML/Graphics.hpp"
#include <iostream>

#define PI 3.14159265359

struct Line {
    int pointA;
    int pointB;
    sf::RectangleShape line;
};

struct Phase {
    std::string hint;
    std::string controls;
};

enum LineMode {ToPrevious, ToFirst};

enum PhaseIndex {One, OneHalf, Two, Three};

void initPhases(std::vector<Phase>&);
void createPoint(std::vector<sf::CircleShape>& points, float x, float y);
void createLine(std::vector<sf::CircleShape>& points, std::vector<Line>& lines, LineMode lineMode);
double calculateDistance(sf::Vector2f a, sf::Vector2f b);
sf::Vector2f calculateMidpoint(sf::Vector2f a, sf::Vector2f b);
double calculateLineAngle(sf::Vector2f a, sf::Vector2f b);

int main()
{
    // Create application window
    int screenWidth = sf::VideoMode::getDesktopMode().width;
    int screenHeight = sf::VideoMode::getDesktopMode().height;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Triangulator-2D", sf::Style::Fullscreen, settings);
    window.setVerticalSyncEnabled(true);
    window.setKeyRepeatEnabled(false);

    // Initialize phases
    std::vector<Phase> phase = {};
    initPhases(phase);
    int active = One;

    // Initialize vectors containing the primitives
    std::vector<sf::CircleShape> points = {};
    std::vector<Line> lines = {};

    // Header
    sf::RectangleShape header(sf::Vector2f(screenWidth, 50.f));
    header.setFillColor(sf::Color(40, 40, 40));

    // Footer
    sf::RectangleShape footer(sf::Vector2f(screenWidth, 50.f));
    footer.setOrigin(0, 50.f);
    footer.setPosition(0, screenHeight);
    footer.setFillColor(sf::Color(40, 40, 40));

    // Import text font
    sf::Font font;
    if (!font.loadFromFile("RussoOne-Regular.ttf"))
    {
        std::cout << "Error loading font" << std::endl;
    }
    
    // Initialize texts
    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("Triangulator-2D v1.0.0");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(10, 7);

    sf::Text devText;
    devText.setFont(font);
    devText.setString("<> by raynesz.dev w/ SFML");
    devText.setCharacterSize(24);
    devText.setFillColor(sf::Color::White);
    devText.setOrigin(devText.getLocalBounds().width, devText.getLocalBounds().height / 2);
    devText.setPosition(screenWidth - 10, 20);

    sf::Text fpsText;
    fpsText.setFont(font);
    fpsText.setCharacterSize(24);
    fpsText.setFillColor(sf::Color::White);
    fpsText.setString("FPS: 0");
    fpsText.setPosition(10, 50);

    sf::Text debugText;
    debugText.setFont(font);
    debugText.setCharacterSize(24);
    debugText.setFillColor(sf::Color::White);
    debugText.setString("Points: " + std::to_string(points.size()) + "  |  Lines: " + std::to_string(lines.size()));
    debugText.setOrigin(0, debugText.getLocalBounds().height);
    debugText.setPosition(10, screenHeight-60);

    sf::Text hintText;
    hintText.setFont(font);
    hintText.setCharacterSize(24);
    hintText.setFillColor(sf::Color::White);
    hintText.setString(phase[active].hint);
    hintText.setOrigin(hintText.getLocalBounds().width / 2, hintText.getLocalBounds().height / 2);
    hintText.setPosition(screenWidth / 2.f, 20);

    sf::Text controlsText;
    controlsText.setFont(font);
    controlsText.setCharacterSize(22);
    controlsText.setFillColor(sf::Color::White);
    controlsText.setString(phase[active].controls);
    controlsText.setOrigin(controlsText.getLocalBounds().width / 2, controlsText.getLocalBounds().height / 2);
    controlsText.setPosition(screenWidth / 2.f, screenHeight - 28);

    // Booleans controling what appears on screen
    bool showPoints = true;
    bool showLines = true;
    bool showTriangles = true;
    bool showFps = true;

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
                    if (active == One) {
                        sf::Vector2i position = sf::Mouse::getPosition();
                        if (position.y > 50 && position.y < (screenHeight - 50)) {
                            createPoint(points, position.x, position.y);
                            if (points.size() > 1) createLine(points, lines, ToPrevious);
                        }
                    }
                }
                else if (event.mouseButton.button == sf::Mouse::Right) {
                    if (active == One) {
                        if (!lines.empty()) lines.pop_back();
                        if (!points.empty()) points.pop_back();
                    }
                }
                debugText.setString("Points: " + std::to_string(points.size()) + "  |  Lines: " + std::to_string(lines.size()));
            }
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Space) {
                    if (active == One) {
                        createLine(points, lines, ToFirst);
                        active = Two;
                    }
                }
                else if (event.key.code == sf::Keyboard::BackSpace) {
                    active = One;
                    points.clear();
                    lines.clear();
                }
                else if (event.key.code == sf::Keyboard::Enter) {
                    
                }
                else if (event.key.code == sf::Keyboard::Q) {
                    showPoints = !showPoints;
                }
                else if (event.key.code == sf::Keyboard::W) {
                    showLines = !showLines;
                }
                else if (event.key.code == sf::Keyboard::E) {
                    showTriangles = !showTriangles;
                }
                else if (event.key.code == sf::Keyboard::F) {
                    showFps = !showFps;
                }
                else if (event.key.code == sf::Keyboard::N) {
                    std::cout << "Points: " << points.size() << "  |  Lines: " << lines.size() << std::endl;
                }
                else if (event.key.code == sf::Keyboard::M) {
                    
                }
                else if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
                hintText.setString(phase[active].hint);
                controlsText.setString(phase[active].controls);
                hintText.setOrigin(hintText.getLocalBounds().width / 2, hintText.getLocalBounds().height / 2);
                controlsText.setOrigin(controlsText.getLocalBounds().width / 2, controlsText.getLocalBounds().height / 2);
                debugText.setString("Points: " + std::to_string(points.size()) + "  |  Lines: " + std::to_string(lines.size()));
            }
        }
        
        // RENDER
        window.clear(sf::Color(18, 33, 43)); // Color background
        if (showLines) for (int i = 0; i < lines.size(); i++) window.draw(lines[i].line);
        if (showPoints) for (int i = 0; i < points.size(); i++) window.draw(points[i]);
        window.draw(header);
        window.draw(footer);
        window.draw(titleText);
        window.draw(hintText);
        window.draw(devText);
        if (showFps) window.draw(fpsText);
        window.draw(debugText);
        window.draw(controlsText);
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
        Line line{ (lineMode == ToPrevious) ? points.size() - 2 : 0 , points.size() - 1 };

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

double calculateDistance(sf::Vector2f a, sf::Vector2f b) {
    return sqrt(pow((a.x - b.x), 2) + pow((a.y - b.y), 2));
}

sf::Vector2f calculateMidpoint(sf::Vector2f a, sf::Vector2f b) {
    return sf::Vector2f((b.x + a.x)/2, (b.y + a.y) / 2);
}

double calculateLineAngle(sf::Vector2f a, sf::Vector2f b) {
    return atan2(a.y - b.y, a.x - b.x) * 180.0 / PI;
}

void initPhases(std::vector<Phase>& phase) {
    std::vector<std::string> hints = { "Draw the area you want to triangulate" , "Lines must not intersect" , "Proceed to Triangulation" , "Triangulation complete" };
    std::vector<std::string> controls = { "Left click: Place point | Right click: Erase point | Spacebar: Close loop | Backspace: Reset | Escape: Close application" ,
        "Backspace: Reset | Escape: Close application" , "Enter: Triangulate | Backspace: Reset | Escape: Close application" , 
        "Q: Show / Hide points | W: Show / Hide lines | E: Show / Hide triangles | Backspace: Reset | Escape: Close application" };
    for (int i = 0; i < 4; i++) phase.push_back(Phase{ hints[i], controls[i] });
}
