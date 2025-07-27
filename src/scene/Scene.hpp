#ifndef TRIANGULATION_SCENE_SCENE_HPP
#define TRIANGULATION_SCENE_SCENE_HPP

#include "scene/Camera.hpp"

namespace triangulation {
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