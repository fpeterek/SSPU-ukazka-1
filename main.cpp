#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cmath>


float momentumDecrease(float orig) {
    constexpr float decelerationFactor = 30.f;
    return std::abs(orig) / decelerationFactor * (orig < 0.f ? -1.f : 1.f);
}

int main() {

    // Init window
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Test", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // Init constants
    constexpr float velocity = 2.f;
    constexpr float rotateAcceleration = 0.1f;
    constexpr float maxRotateVelocity = 10.f;
    const sf::Color orig = sf::Color::White;
    const sf::Color hover = sf::Color::Black;
    const sf::Color background = sf::Color(200, 100, 100);

    // Calculate scale
    const float scale = window.getSize().x / 800.f;

    // Init rect
    sf::RectangleShape rect(sf::Vector2f(50, 50));
    float rotateMomentum = 0;
    bool momentumChanged = false;
    rect.setOrigin(25, 25);
    rect.setScale(scale, scale);
    rect.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

    // sf::Event variable used to hold polled events
    sf::Event ev;

    // Main loop
    while (window.isOpen()) {

        // Handle events
        while (window.pollEvent(ev)) {

            if (ev.type == sf::Event::EventType::Closed) {
                window.close();
                break;
            }

            if (ev.type == sf::Event::EventType::KeyPressed) {
                if (ev.key.code == sf::Keyboard::Escape) {
                    window.close();
                    break;
                }
            }

        }

        // Handle keyboard
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            rect.move(sf::Vector2f(0, velocity));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            rect.move(sf::Vector2f(0, -velocity));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            rect.move(sf::Vector2f(-velocity, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            rect.move(sf::Vector2f(velocity, 0));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            rotateMomentum -= rotateAcceleration;
            if (rotateMomentum < 0.f) {
                momentumChanged = true;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
            rotateMomentum += rotateAcceleration;
            if (rotateMomentum > 0.f) {
                momentumChanged = true;
            }  
        }

        rotateMomentum = std::min(rotateMomentum, maxRotateVelocity);
        rotateMomentum = std::max(rotateMomentum, -maxRotateVelocity);

        // Handle rotation
        if (not momentumChanged) {
            rotateMomentum -= momentumDecrease(rotateMomentum);
        }
        if (std::abs(rotateMomentum) < 0.05f) {
            rotateMomentum = 0.f;
        }
        momentumChanged = false;
        rect.rotate(rotateMomentum);

        // Check if mouse cursor is hovering over rect
        const sf::Vector2i pos = sf::Mouse::getPosition(window);

        if (rect.getGlobalBounds().contains(pos.x, pos.y)) {
            rect.setFillColor(hover);
        } else {
            rect.setFillColor(orig);
        }

        // Render
        window.clear(background);
        window.draw(rect);
        window.display();

    }

}
