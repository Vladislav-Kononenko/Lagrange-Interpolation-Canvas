#include <SFML/Graphics.hpp>
#include "Canvas.hpp"
#include <cstdio>

int main() {

    freopen("/dev/null", "w", stderr);

    const unsigned width = 1000;
    const unsigned height = 700;

    sf::RenderWindow window(
    sf::VideoMode({width, height}), "Lagrange Canvas (SFML)");
    window.setFramerateLimit(60);

    Canvas canvas(window.getSize());

    while (window.isOpen()) {
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

        window.clear(sf::Color(20, 20, 20));
        canvas.draw(window);
        window.display();
    }
    return 0;
}
