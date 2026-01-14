#include <SFML/Graphics.hpp>
#include "Canvas.hpp"

int main() {
    const unsigned width = 1000;
    const unsigned height = 700;

    sf::RenderWindow window(
#if defined(SFML_VERSION_MAJOR) && SFML_VERSION_MAJOR >= 3
        sf::VideoMode({width, height})
#else
        sf::VideoMode(width, height)
#endif
        , "Lagrange Canvas (SFML)");
    window.setFramerateLimit(60);

    Canvas canvas(window.getSize());

    while (window.isOpen()) {
#if defined(SFML_VERSION_MAJOR) && SFML_VERSION_MAJOR >= 3
        while (const std::optional ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                window.close();
            }
            if (const auto* key = ev->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) window.close();
                if (key->code == sf::Keyboard::Key::C) canvas.clear();
                if (key->code == sf::Keyboard::Key::Z) canvas.removeLast();
            }
            canvas.handleEvent(*ev, window);
        }
#else
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) window.close();
                if (event.key.code == sf::Keyboard::C) canvas.clear();
                if (event.key.code == sf::Keyboard::Z) canvas.removeLast();
            }
            canvas.handleEvent(event, window);
        }
#endif

        window.clear(sf::Color(20, 20, 20));
        canvas.draw(window);
        window.display();
    }
    return 0;
}
