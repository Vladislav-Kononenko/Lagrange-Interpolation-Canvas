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
: size_(size), curve_(sf::PrimitiveType::LineStrip)
{
}

void Canvas::addPoint(const sf::Vector2f& p) {
    // Избегайте точных дубликатов, чтобы параметры оставались четко определенными
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
    if (const auto* mb = ev.getIf<sf::Event::MouseButtonPressed>()) {
        if (mb->button == sf::Mouse::Button::Left) {
            const sf::Vector2i mp = mb->position;
            sf::Vector2f world = window.mapPixelToCoords(mp);
            addPoint(sf::Vector2f(static_cast<float>(world.x), static_cast<float>(world.y)));
        } else if (mb->button == sf::Mouse::Button::Right) {
            removeLast();
        }
    }
}

void Canvas::draw(sf::RenderTarget& target) {
    // Сначала нарисуйте кривую
    if (curve_.getVertexCount() >= 2) {
        target.draw(curve_);
    }

    // Отображайте точки в виде маленьких кружков
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
        // Все точки одинаковы; сохраняйте нули
    } else {
        for (auto& v : t_) v /= total; // нормализовать до диапазона [0,1]
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

    // Подготовьте массивы значений.
    vector<double> xs(n), ys(n);
    for (std::size_t i = 0; i < n; ++i) {
        xs[i] = static_cast<double>(points_[i].x);
        ys[i] = static_cast<double>(points_[i].y);
    }

    // Разрешение выборки пропорционально ширине
    const unsigned sampleCount = std::max(2u, size_.x * 2u);
    curve_.resize(sampleCount);
    curve_.setPrimitiveType(sf::PrimitiveType::LineStrip);
    for (unsigned k = 0; k < sampleCount; ++k) {
        double t = (sampleCount == 1) ? 0.0 : static_cast<double>(k) / static_cast<double>(sampleCount - 1);
        double px = interp_.evaluate(xs, t);
        double py = interp_.evaluate(ys, t);
        curve_[k].position = sf::Vector2f(static_cast<float>(px), static_cast<float>(py));
        curve_[k].color = sf::Color(40, 180, 255);
    }
}
