#include "AdvancingFront.hpp"
#include "QuickHull.hpp"
#include <queue>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace triangulation {

    AdvancingFront::Edge::Edge () {}

    AdvancingFront::Edge::Edge (glm::vec2 _point1, glm::vec2 _point2, bool _is_in_frontier) : point1(_point1), point2(_point2), is_in_frontier(_is_in_frontier) {}

    bool AdvancingFront::Edge::operator == (Edge const& edge) const {

        Edge inverse_edge(edge.point2, edge.point1, false);

        return glm::all(glm::equal(this->point1, edge.point1) && glm::equal(this->point2, edge.point2)) || glm::all(glm::equal(this->point1, inverse_edge.point1) && glm::equal(this->point2, inverse_edge.point2));

    }

    std::vector<std::shared_ptr<AdvancingFront::Edge>> AdvancingFront::compute_initial_frontier (std::vector<glm::vec2> const& points) {

        std::vector<std::shared_ptr<Edge>> initial_frontier;
        std::vector<glm::vec2> convex_hull_points;
        QuickHull quickhull;

        convex_hull_points = quickhull.compute_hull(points);
        initial_frontier.reserve(convex_hull_points.size());

        for (int i = convex_hull_points.size() - 1; i >= 0; --i) {

            initial_frontier.emplace_back(new Edge(convex_hull_points[i], (i == 0) ? convex_hull_points.back() : convex_hull_points[i-1], true));

        }

        return initial_frontier;

    }

    std::optional<glm::vec2> AdvancingFront::find_candidate_point (Edge const& edge, std::vector<std::shared_ptr<Edge>> const& edges, std::vector<glm::vec2> const& points) {

        std::optional<glm::vec2> candidate_point;
        glm::vec2
            edge_vector = edge.point2 - edge.point1;
        float
            angle, triangle_area,
            max_angle = -INFINITY,
            min_triangle_area = INFINITY;
        bool is_a_valid_point;
        std::size_t i;

        // Finding the valid point with the minimum distance from the edge.
        for (auto const& point : points) {

            if (glm::cross(glm::vec3(edge_vector, 0.0f), glm::vec3(point - edge.point1, 0.0f)).z > 0) {

                // Checking if it is a valid point (no intersection).
                is_a_valid_point = true;
                i = 0;
                while (is_a_valid_point && i < edges.size()) {

                    is_a_valid_point = !check_intersection(*edges[i], Edge(edge.point1, point, false)) && !check_intersection(*edges[i], Edge(edge.point2, point, false));
                    ++i;

                }

                if (is_a_valid_point) {

                    angle = glm::angle(edge.point1 - point, edge.point2 - point);

                    if (angle > max_angle) {

                        max_angle = angle;
                        candidate_point = point;

                    } else if (angle == max_angle) {

                        triangle_area = glm::cross(glm::vec3(edge_vector, 0.0f), glm::vec3(point - edge.point1, 0.0f)).z/2.0f;

                        if (triangle_area < min_triangle_area) {

                            min_triangle_area = triangle_area;
                            candidate_point = point;

                        }

                    }

                }

            }

        }

        return candidate_point;

    }

    bool AdvancingFront::check_intersection (Edge const& e1, Edge const& e2) {

        return
            glm::cross(glm::vec3(e1.point2 - e1.point1, 0.0f), glm::vec3(e2.point1 - e1.point1, 0.0f)).z * glm::cross(glm::vec3(e1.point2 - e1.point1, 0.0f), glm::vec3(e2.point2 - e1.point1, 0.0f)).z < 0
            &&
            glm::cross(glm::vec3(e2.point2 - e2.point1, 0.0f), glm::vec3(e1.point1 - e2.point1, 0.0f)).z * glm::cross(glm::vec3(e2.point2 - e2.point1, 0.0f), glm::vec3(e1.point2 - e2.point1, 0.0f)).z < 0;

    }

    std::shared_ptr<AdvancingFront::Edge> AdvancingFront::find_edge_pointer (glm::vec2 point1, glm::vec2 point2, std::vector<std::shared_ptr<Edge>> const& frontier) {

        for (auto const& e : frontier) {

            if (Edge(point1, point2, false) == *e) {

                return e;

            }

        }

        return nullptr;

    }

    std::vector<glm::vec2> AdvancingFront::compute_triangulation (std::vector<glm::vec2> const& points) {

        std::vector<glm::vec2> triangles;
        std::vector<std::shared_ptr<Edge>> frontier = AdvancingFront::compute_initial_frontier(points);
        std::queue<std::shared_ptr<Edge>> edges_queue;
        std::shared_ptr<Edge> current_edge, new_edge1, new_edge2;
        std::optional<glm::vec2> candidate_point;

        for (auto const& edge : frontier) {

            edges_queue.push(edge);

        }

        while (!edges_queue.empty()) {

            current_edge = std::move(edges_queue.front());
            edges_queue.pop();

            if (current_edge->is_in_frontier) {

                candidate_point = AdvancingFront::find_candidate_point(*current_edge, frontier, points);

                if (candidate_point.has_value()) {

                    triangles.push_back(current_edge->point1);
                    triangles.push_back(current_edge->point2);
                    triangles.push_back(candidate_point.value());

                    // Updating the frontier.
                    current_edge->is_in_frontier = false;

                    new_edge1 = AdvancingFront::find_edge_pointer(current_edge->point1, candidate_point.value(), frontier);
                    if (new_edge1 == nullptr) {

                        new_edge1 = std::make_shared<Edge>(current_edge->point1, candidate_point.value(), true);
                        frontier.push_back(new_edge1);
                        edges_queue.push(new_edge1);

                    } else {

                        new_edge1->is_in_frontier = false;

                    }

                    new_edge2 = AdvancingFront::find_edge_pointer(candidate_point.value(), current_edge->point2, frontier);
                    if (new_edge2 == nullptr) {

                        new_edge2 = std::make_shared<Edge>(candidate_point.value(), current_edge->point2, true);
                        frontier.push_back(new_edge2);
                        edges_queue.push(new_edge2);

                    } else {

                        new_edge2->is_in_frontier = false;

                    }

                }

            }

        }

        return triangles;

    }

}