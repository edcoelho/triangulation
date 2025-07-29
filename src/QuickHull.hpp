#ifndef TRIANGULATION_QUICKHULL_HPP
#define TRIANGULATION_QUICKHULL_HPP

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <vector>
#include <utility>
#include <glm/vec2.hpp>

namespace triangulation {

    class QuickHull {

        private:

            static std::vector<glm::vec2> compute_hull (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high);

            static std::pair<std::vector<glm::vec2>, std::vector<glm::vec2>> divide (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high);

            static std::vector<glm::vec2> combine (std::vector<glm::vec2> const& points1, std::vector<glm::vec2> const& points2);

        public:

            static std::vector<glm::vec2> compute_hull (std::vector<glm::vec2> const& points);

    };

}

#endif