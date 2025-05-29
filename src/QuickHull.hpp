#ifndef CONVEX_HULL_QUICKHULL_HPP
#define CONVEX_HULL_QUICKHULL_HPP

#include <vector>
#include <utility>
#include "math/Vector2.hpp"

namespace convex_hull {

    class QuickHull {

        private:

            std::vector<math::Vector2> compute_hull (std::vector<math::Vector2> const& points, math::Vector2 const& pivot_low, math::Vector2 const& pivot_high) const;

            std::pair<std::vector<math::Vector2>, std::vector<math::Vector2>> divide (std::vector<math::Vector2> const& points, math::Vector2 const& pivot_low, math::Vector2 const& pivot_high) const;

            std::vector<math::Vector2> combine (std::vector<math::Vector2> const& points1, std::vector<math::Vector2> const& points2) const;

        public:

            std::vector<math::Vector2> compute_hull (std::vector<math::Vector2> const& points) const;

    };

}

#endif