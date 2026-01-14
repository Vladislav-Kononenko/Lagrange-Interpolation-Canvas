#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Interpolator.hpp"

class Canvas {
public:
    explicit Canvas(const sf::Vector2u& size);

    void addPoint(const sf::Vector2f& p);
    void clear();
    void removeLast();

    void handleEvent(const sf::Event& ev, const sf::RenderWindow& window);
    void draw(sf::RenderTarget& target);

private:
    void rebuildParameters();
    void rebuildCurve();

    sf::Vector2u size_;
    std::vector<sf::Vector2f> points_;
    std::vector<double> t_;
    BarycentricLagrange interp_;
    sf::VertexArray curve_;
};

