#ifndef TRIANGULATION_ADVANCINGFRONT_HPP
#define TRIANGULATION_ADVANCINGFRONT_HPP

#include <vector>
#include <memory>
#include <optional>
#include <glm/vec2.hpp>

namespace triangulation {

    class AdvancingFront {

        private:

            struct Edge {

                glm::vec2 point1, point2;
                bool is_in_frontier;

                Edge ();
                Edge (glm::vec2 _point1, glm::vec2 _point2, bool _is_in_frontier);

                bool operator == (Edge const& edge) const;

            };

            static std::vector<std::shared_ptr<Edge>> compute_initial_frontier (std::vector<glm::vec2> const& points);

            static std::optional<glm::vec2> find_candidate_point (Edge const& edge, std::vector<std::shared_ptr<Edge>> const& edges, std::vector<glm::vec2> const& points);

            static bool check_intersection (Edge const& e1, Edge const& e2);

            static std::shared_ptr<Edge> find_edge_pointer (glm::vec2 point1, glm::vec2 point2, std::vector<std::shared_ptr<Edge>> const& frontier);

        public:

            static std::vector<glm::vec2> compute_triangulation (std::vector<glm::vec2> const& points) ;

    };

}

#endif