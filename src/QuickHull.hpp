#ifndef CONVEX_HULL_QUICKHULL_HPP
#define CONVEX_HULL_QUICKHULL_HPP

#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif

#include <vector>
#include <utility>
#include <glm/vec2.hpp>
#include "AnimationFrame.hpp"

namespace convex_hull {

    class QuickHull {

        private:

            std::vector<glm::vec2> compute_hull (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high, std::vector<AnimationFrame>& frames) const;

            std::pair<std::vector<glm::vec2>, std::vector<glm::vec2>> divide (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high) const;

            std::vector<glm::vec2> combine (std::vector<glm::vec2> const& points1, std::vector<glm::vec2> const& points2) const;

        public:

            struct QuickHullResult {

                std::vector<glm::vec2> vertices;
                std::vector<AnimationFrame> frames;

            };

            QuickHullResult compute_hull (std::vector<glm::vec2> const& points) const;

    };

}

#endif