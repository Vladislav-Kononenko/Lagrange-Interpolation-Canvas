#include "Interpolator.hpp"
#include <cmath>

using std::vector;

void BarycentricLagrange::setNodes(const vector<double>& t) {
    t_ = t;
    w_.clear();
}

void BarycentricLagrange::computeWeights() {
    const std::size_t n = t_.size();
    w_.assign(n, 0.0);
    if (n == 0) return;
    for (std::size_t i = 0; i < n; ++i) {
        double wi = 1.0;
        for (std::size_t j = 0; j < n; ++j) {
            if (i == j) continue;
            double diff = t_[i] - t_[j];
            wi *= diff;
        }
        w_[i] = (wi != 0.0) ? 1.0 / wi : 0.0;
    }
}

double BarycentricLagrange::evaluate(const vector<double>& f, double t) const {
    const std::size_t n = t_.size();
    if (n == 0 || f.size() != n) return 0.0;
    if (n == 1) return f[0];

    // Математически барицентрическая формула содержит члены вида 1/(t - t_i).
    // Если t == t_i, будет деление на ноль.
    const double eps = 1e-12;
    for (std::size_t i = 0; i < n; ++i) {
        if (std::abs(t - t_[i]) <= eps) return f[i];
    }

    double num = 0.0;
    double den = 0.0;
    for (std::size_t i = 0; i < n; ++i) {
        double inv = 1.0 / (t - t_[i]);
        double term = w_[i] * inv;
        num += term * f[i];
        den += term;
    }
    if (den == 0.0) return 0.0;
    return num / den;
}
