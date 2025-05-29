#include "QuickHull.hpp"
#include "math/Vector2.hpp"
#include <iostream>
#include <cmath>
#include <vector>

void print_hull(const std::vector<convex_hull::math::Vector2>& input, const std::vector<convex_hull::math::Vector2>& hull) {
    std::cout << "A: ";
    for (const auto& p : input)
        std::cout << "(" << p.x << ", " << p.y << ") ";
    std::cout << "\nConvex Hull of A: ";
    for (const auto& p : hull)
        std::cout << "(" << p.x << ", " << p.y << ") ";
    std::cout << "\n\n";
}
using convex_hull::math::Vector2;

int main() {
    convex_hull::QuickHull qh;

    // Teste 1: Caso original (retângulo)
    {
        std::vector<Vector2> points = {
            {0, 3}, {2, 2}, {1, 1}, {2, 1}, {3, 0}, {0, 0}, {3, 3}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Teste 1 - Retângulo:\n";
        print_hull(points, hull);
    }

    // Teste 2: Pontos colineares
    {
        std::vector<Vector2> points = {
            {0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Teste 2 - Pontos colineares:\n";
        print_hull(points, hull);
    }

    // Teste 3: Concavidades
    {
        std::vector<Vector2> points = {
            {0, 3}, {1, 1}, {2, 3}, {3, 1}, {2, 2}, {1, 0}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Teste 3 - Estrela com concavidades:\n";
        print_hull(points, hull);
    }

    // Teste 4: Duplicatas
    {
        std::vector<Vector2> points = {
            {0, 0}, {1, 1}, {0, 0}, {2, 2}, {3, 0}, {1, 1}
        };
        auto hull = qh.compute_hull(points);
        std::cout << "Teste 4 - Com duplicatas:\n";
        print_hull(points, hull);
    }

    // Teste 5: Distribuição circular (simula um círculo)
    {
        std::vector<Vector2> points;
        int n = 20;
        float radius = 5.0f;
        for (int i = 0; i < n; ++i) {
            float angle = 2 * 3.14159f * i / n;
            points.emplace_back(radius * std::cos(angle), radius * std::sin(angle));
        }
        auto hull = qh.compute_hull(points);
        std::cout << "Teste 5 - Distribuição circular:\n";
        print_hull(points, hull);
    }

    return EXIT_SUCCESS;
}