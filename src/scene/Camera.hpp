#ifndef CONVEX_HULL_SCENE_CAMERA_HPP
#define CONVEX_HULL_SCENE_CAMERA_HPP

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace convex_hull {
namespace scene {

enum ProjectionType {

    ORTHOGRAPHIC,
    PERSPECTIVE

};

class Camera {

    private:

        glm::vec3 position;
        glm::vec3 look_at;
        glm::vec3 view_up;
        ProjectionType projection_type;
        float near, far, bottom, top, left, right;
        // World to camera matrix
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;

        bool need_to_update_view_matrix, need_to_update_projection_matrix;
        void update_view_matrix();
        void update_projection_matrix();

    public:

        Camera(glm::vec3 _position = glm::vec3(0.0f), glm::vec3 _look_at = glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3 _view_up = glm::vec3(0.0f, 1.0f, 0.0f), ProjectionType _projection_type = ProjectionType::ORTHOGRAPHIC, float _near = 0.1f, float _far = 1000.0f, float _bottom = -500.0f, float _top = 500.0f, float _left = -500.0f, float _right = 500.0f);

        glm::vec3 get_position() const;
        void set_position(glm::vec3 pos);

        glm::vec3 get_look_at() const;
        void set_look_at(glm::vec3 pos);

        glm::vec3 get_view_up() const;
        void set_view_up(glm::vec3 pos);

        ProjectionType get_projection_type() const;
        void set_projection_type(ProjectionType type);

        float get_near() const;
        void set_near(float scalar);

        float get_far() const;
        void set_far(float scalar);

        float get_bottom() const;
        void set_bottom(float scalar);

        float get_top() const;
        void set_top(float scalar);

        float get_left() const;
        void set_left(float scalar);

        float get_right() const;
        void set_right(float scalar);

        glm::mat4 const& get_view_matrix();

        glm::mat4 const& get_projection_matrix();

};

}
}

#endif
