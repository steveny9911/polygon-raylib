// g++ -I/opt/homebrew/Cellar/sfml/2.6.1/include -o rectangle_animation rectangle_animation.cpp -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

int main() {
    // Define the rectangle corners
    const float x1 = 0, y1 = 0; // Top-left corner
    const float x2 = 6, y2 = 4; // Bottom-right corner

    const float L = x2 - x1; // Length (width)
    const float H = y2 - y1; // Height
    const float P = 2 * (L + H); // Perimeter
    const float d = P / 100; // Distance traveled per frame, arbitrary for smoothness

    // Initialize the starting position at the bottom-left corner
    float current_x = x1, current_y = y2; // Starting at the bottom-left
    float t = 0.0f; // Time variable for tracking position

    // Create a window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Rectangle Animation");

    // Set up rectangle shape
    sf::RectangleShape rectangle(sf::Vector2f(L * 100, H * 100)); // Scale for visibility
    rectangle.setPosition(x1 * 100, y1 * 100); // Scale the position
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Blue);
    rectangle.setOutlineThickness(2);

    // Set up point shape
    sf::CircleShape point(5); // Radius of the point
    point.setFillColor(sf::Color::Red);
    point.setPosition(current_x * 100, current_y * 100); // Scale the position

    // Set up clock to track time
    sf::Clock clock;
    const float interval = 1.0f; // Update every second

    // Main loop
    while (window.isOpen()) {
        // Process events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Check if a second has passed
        if (clock.getElapsedTime().asSeconds() >= interval) {
            clock.restart(); // Reset the clock

            // Update position based on the current time
            t += d; // Increase time

            float t_mod = fmod(t, P); // Wrap time around the perimeter length
            if (t_mod < L) { // Bottom edge
                current_x = x1 + t_mod;
                current_y = y2;
            } else if (t_mod < L + H) { // Right edge
                current_x = x2;
                current_y = y2 - (t_mod - L);
            } else if (t_mod < 2 * L + H) { // Top edge
                current_x = x2 - (t_mod - (L + H));
                current_y = y1;
            } else { // Left edge
                current_x = x1;
                current_y = y1 + (t_mod - (2 * L + H));
            }

            // Update point position
            point.setPosition(current_x * 100, current_y * 100); // Scale the position
        }

        // Clear the window
        window.clear(sf::Color::White);

        // Draw the rectangle and point
        window.draw(rectangle);
        window.draw(point);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
