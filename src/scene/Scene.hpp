#ifndef CONVEX_HULL_SCENE_SCENE_HPP
#define CONVEX_HULL_SCENE_SCENE_HPP

#include "scene/Camera.hpp"

namespace convex_hull {
namespace scene {

class Scene {

    private:

        Camera camera;
        
    public:

        Scene(Camera& camera);

        Camera& get_camera();
        void set_camera(Camera c);

};

}
}

#endif