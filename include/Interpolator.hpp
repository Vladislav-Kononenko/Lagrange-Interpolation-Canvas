// Simple barycentric Lagrange interpolator over scalar parameter t
#pragma once

#include <vector>

class BarycentricLagrange {
public:
    void setNodes(const std::vector<double>& t);
    void computeWeights();

    // Evaluate polynomial at t for data values f (same size as nodes)
    double evaluate(const std::vector<double>& f, double t) const;

    [[nodiscard]] bool ready() const { return !t_.empty() && t_.size() == w_.size(); }
    [[nodiscard]] std::size_t size() const { return t_.size(); }

private:
    std::vector<double> t_;
    std::vector<double> w_;
};

