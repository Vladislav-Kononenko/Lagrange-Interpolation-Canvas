// Простой барицентрический интерполятор Лагранжа по скалярному параметру t
#pragma once

#include <vector>

class BarycentricLagrange {

public:
    void setNodes(const std::vector<double>& t);
    void computeWeights();

    // Вычислить значение полинома в точке t, используя заданные значения f (размерность массива f совпадает с количеством узлов).
    double evaluate(const std::vector<double>& f, double t) const;
    // Возвращаемое значение не должно игнорироваться
    [[nodiscard]] bool ready() const { return !t_.empty() && t_.size() == w_.size(); }
    [[nodiscard]] std::size_t size() const { return t_.size(); }

private:
    std::vector<double> t_; // вершины
    std::vector<double> w_; // барицентрические веса
};
