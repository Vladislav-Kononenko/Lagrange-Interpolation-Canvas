#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

struct Point {
    double x;
    double y;
};


vector<Point> generate_points(
    int m,
    double x_min,
    double x_max
) {
    vector<Point> points;

    if (m < 2) {
        return points;
    }

    double h = (x_max - x_min) / (m - 1);

    for (int i = 0; i < m; ++i) {
        double x = x_min + i * h;
        double y = x * x;

        points.push_back({x, y});
    }

    return points;
}


int main() {
    for (const auto& point : generate_points(3, 100, 1000)) {
        cout << fixed << setprecision(0);
        cout << "Point: (" << point.x << ", " << point.y << ")" << endl;
    }

    return 0;
}