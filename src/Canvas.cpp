#include "Canvas.hpp"

#include <cmath>
#include <algorithm>

using std::vector;

namespace {
    float length(const sf::Vector2f& a, const sf::Vector2f& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx*dx + dy*dy);
    }
}

Canvas::Canvas(const sf::Vector2u& size)
#if defined(SFML_VERSION_MAJOR) && SFML_VERSION_MAJOR >= 3
    : size_(size), curve_(sf::PrimitiveType::LineStrip)
#else
    : size_(size), curve_(sf::LineStrip)
#endif
{
}

void Canvas::addPoint(const sf::Vector2f& p) {
    // Avoid exact duplicates to keep parameters well-defined
    if (!points_.empty()) {
        if (length(points_.back(), p) < 1e-6f) return;
    }
    points_.push_back(p);
    rebuildParameters();
    rebuildCurve();
}

void Canvas::clear() {
    points_.clear();
    t_.clear();
    curve_.clear();
}

void Canvas::removeLast() {
    if (!points_.empty()) {
        points_.pop_back();
        rebuildParameters();
        rebuildCurve();
    }
}

void Canvas::handleEvent(const sf::Event& ev, const sf::RenderWindow& window) {
#if defined(SFML_VERSION_MAJOR) && SFML_VERSION_MAJOR >= 3
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            const sf::Vector2i mp = mb->position;
            addPoint(sf::Vector2f(static_cast<float>(mp.x), static_cast<float>(mp.y)));
        } else if (mb->button == sf::Mouse::Button::Right) {
            removeLast();
        }
    }
#else
    if (ev.type == sf::Event::MouseButtonPressed) {
        if (ev.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mp = sf::Mouse::getPosition(window);
            addPoint(sf::Vector2f(static_cast<float>(mp.x), static_cast<float>(mp.y)));
        } else if (ev.mouseButton.button == sf::Mouse::Right) {
            removeLast();
        }
    }
#endif
}

void Canvas::draw(sf::RenderTarget& target) {
    // Draw curve first
    if (curve_.getVertexCount() >= 2) {
        target.draw(curve_);
    }

    // Draw points as small circles
    sf::CircleShape c(4.f);
    c.setOrigin(sf::Vector2f{4.f, 4.f});
    c.setFillColor(sf::Color::Red);
    for (const auto& p : points_) {
        c.setPosition(p);
        target.draw(c);
    }
}

void Canvas::rebuildParameters() {
    const std::size_t n = points_.size();
    t_.assign(n, 0.0);
    if (n == 0) {
        interp_.setNodes(t_);
        interp_.computeWeights();
        return;
    }
    if (n == 1) {
        t_[0] = 0.0;
        interp_.setNodes(t_);
        interp_.computeWeights();
        return;
    }

    t_[0] = 0.0;
    for (std::size_t i = 1; i < n; ++i) {
        t_[i] = t_[i - 1] + static_cast<double>(length(points_[i], points_[i - 1]));
    }
    double total = t_.back();
    if (total <= 0.0) {
        // All points are the same; keep zeros
    } else {
        for (auto& v : t_) v /= total; // normalize to [0,1]
    }

    interp_.setNodes(t_);
    interp_.computeWeights();
}

void Canvas::rebuildCurve() {
    const std::size_t n = points_.size();
    if (n < 2 || !interp_.ready()) {
        curve_.clear();
        return;
    }

    // Prepare value arrays
    vector<double> xs(n), ys(n);
    for (std::size_t i = 0; i < n; ++i) {
        xs[i] = static_cast<double>(points_[i].x);
        ys[i] = static_cast<double>(points_[i].y);
    }

    // Sample resolution proportional to width
    const unsigned sampleCount = std::max(2u, size_.x * 2u);
    curve_.resize(sampleCount);
#if defined(SFML_VERSION_MAJOR) && SFML_VERSION_MAJOR >= 3
    curve_.setPrimitiveType(sf::PrimitiveType::LineStrip);
#else
    curve_.setPrimitiveType(sf::LineStrip);
#endif
    for (unsigned k = 0; k < sampleCount; ++k) {
        double t = (sampleCount == 1) ? 0.0 : static_cast<double>(k) / static_cast<double>(sampleCount - 1);
        double px = interp_.evaluate(xs, t);
        double py = interp_.evaluate(ys, t);
        curve_[k].position = sf::Vector2f(static_cast<float>(px), static_cast<float>(py));
        curve_[k].color = sf::Color(40, 180, 255);
    }
}
