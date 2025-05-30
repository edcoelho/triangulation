#include "QuickHull.hpp"
#include <glm/vec2.hpp>
#include <iostream>
#include <cmath>
#include <vector>

void print_hull(const std::vector<glm::vec2>& input, const std::vector<glm::vec2>& hull) {
    std::cout << "A: ";
    for (const auto& p : input)
        std::cout << "(" << p.x << ", " << p.y << ") ";
    std::cout << "\nConvex Hull of A: ";
    for (const auto& p : hull)
        std::cout << "(" << p.x << ", " << p.y << ") ";
    std::cout << "\n\n";
}

int main() {
    convex_hull::QuickHull qh;

    // Test 1
    {
        std::vector<glm::vec2> points = {
            {0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0}, {0, 0}, {3, 3}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Test 1\n";
        print_hull(points, hull);
    }

    // Test 2
    {
        std::vector<glm::vec2> points = {
            {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Test 2\n";
        print_hull(points, hull);
    }

    // Test 3
    {
        std::vector<glm::vec2> points = {
            {0, 3}, {1, 1}, {2, 3}, {3, 1}, {2, 2}, {1, 0}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Test 3\n";
        print_hull(points, hull);
    }

    // Test 4
    {
        std::vector<glm::vec2> points = {
            {0, 0}, {1, 1}, {0, 0}, {2, 2}, {3, 0}, {1, 1}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Test 4\n";
        print_hull(points, hull);
    }

    // Test 5
    {
        std::vector<glm::vec2> points;
        int n = 20;
        float radius = 5.0f;
        for (int i = 0; i < n; ++i) {
            float angle = 2 * 3.14159f * i / n;
            points.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
        }
        auto hull = qh.compute_hull(points);
        std::cout << "Test 5\n";
        print_hull(points, hull);
    }

    return EXIT_SUCCESS;
}