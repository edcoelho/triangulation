#ifndef CONVEX_HULL_RENDER_ANIMATION_FRAME_HPP
#define CONVEX_HULL_RENDER_ANIMATION_FRAME_HPP

#include <vector>
#include <glm/vec2.hpp>

namespace convex_hull {

    struct AnimationFrame {

        std::vector<glm::vec2> pivot_edges, hull_edges;

    };

}

#endif