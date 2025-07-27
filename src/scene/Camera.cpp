#include "scene/Camera.hpp"

namespace triangulation {
    namespace scene {

        Camera::Camera(glm::vec3 _position, glm::vec3 _look_at, glm::vec3 _view_up, ProjectionType _projection_type, float _near, float _far, float _bottom, float _top, float _left, float _right) : position(_position), look_at(_look_at), view_up(_view_up), projection_type(_projection_type), near(_near), far(_far), bottom(_bottom), top(_top), left(_left), right(_right), view_matrix(glm::mat4(1.0f)), projection_matrix(1.0f) {

            this->update_view_matrix();
            this->update_projection_matrix();

        }

        glm::vec3 Camera::get_position() const {

            return this->position;

        }
        void Camera::set_position(glm::vec3 pos) {

            this->position = pos;
            this->need_to_update_view_matrix = true;

        }

        glm::vec3 Camera::get_look_at() const {

            return this->look_at;

        }
        void Camera::set_look_at(glm::vec3 pos) {

            this->look_at = pos;
            this->need_to_update_view_matrix = true;

        }

        glm::vec3 Camera::get_view_up() const {

            return this->view_up;

        }
        void Camera::set_view_up(glm::vec3 pos) {

            this->view_up = pos;
            this->need_to_update_view_matrix = true;

        }

        ProjectionType Camera::get_projection_type() const {

            return this->projection_type;

        }
        void Camera::set_projection_type(ProjectionType type) {

            this->projection_type = type;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_near() const {

            return this->near;

        }
        void Camera::set_near(float scalar) {

            this->near = scalar;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_far() const {

            return this->far;

        }
        void Camera::set_far(float scalar) {

            this->far = scalar;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_bottom() const {

            return this->bottom;

        }
        void Camera::set_bottom(float scalar) {

            this->bottom = scalar;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_top() const {

            return this->top;

        }
        void Camera::set_top(float scalar) {

            this->top = scalar;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_left() const {

            return this->left;

        }
        void Camera::set_left(float scalar) {

            this->left = scalar;
            this->need_to_update_projection_matrix = true;

        }

        float Camera::get_right() const {

            return this->right;

        }
        void Camera::set_right(float scalar) {

            this->right = scalar;
            this->need_to_update_projection_matrix = true;

        }

        glm::mat4 const& Camera::get_view_matrix() {

            if (this->need_to_update_view_matrix) this->update_view_matrix();
            return this->view_matrix;

        }

        glm::mat4 const& Camera::get_projection_matrix() {

            if (this->need_to_update_projection_matrix) this->update_projection_matrix();
            return this->projection_matrix;

        }

        void Camera::update_view_matrix() {

            // Remember: GLSL/GLM matrices are stored and initialized in column-major order!

            glm::vec3 k = glm::normalize((this->get_position() - this->get_look_at()));
            glm::vec3 i = glm::normalize(glm::cross(this->get_view_up(), k));
            glm::vec3 j = glm::normalize(glm::cross(k, i));

            for (int index = 0; index < 3; index++) {

                this->view_matrix[index][0] = i[index];
                this->view_matrix[index][1] = j[index];
                this->view_matrix[index][2] = k[index];

            }

            this->view_matrix[3][0] = -glm::dot(i, position);
            this->view_matrix[3][1] = -glm::dot(j, position);
            this->view_matrix[3][2] = -glm::dot(k, position);
            this->view_matrix[3][3] = 1.0f;

            this->need_to_update_view_matrix = false;

        }

        void Camera::update_projection_matrix() {

            // Remember: GLSL/GLM matrices are stored and initialized in column-major order!

            this->projection_matrix = glm::mat4(1.0f);

            glm::vec4 col0, col1, col2, col3;

            if (this->get_projection_type() == ProjectionType::PERSPECTIVE) {

                col0 = glm::vec4(
                    (2.0f * near) / (right - left),
                    0.0f,
                    0.0f, 
                    0.0f
                );

                col1 = glm::vec4(
                    0.0f,
                    (2.0f * near) / (top - bottom),
                    0.0f,
                    0.0f
                );

                col2 = glm::vec4(
                    (right + left) / (right - left),
                    (top + bottom) / (top - bottom),
                    -(far + near) / (far - near),
                    -1.0f
                );

                col3 = glm::vec4(
                    0.0f,
                    0.0f,
                    -(2.0f * far * near) / (far - near),
                    0.0f
                );

            } else {

                col0 = glm::vec4(
                    2.0f / (right - left),
                    0.0f,
                    0.0f,
                    0.0f
                );

                col1 = glm::vec4(
                    0.0f,
                    2.0f / (top - bottom),
                    0.0f,
                    0.0f
                );

                col2 = glm::vec4(
                    0.0f,
                    0.0f,
                    -2.0f / (far - near),
                    0.0f
                );

                col3 = glm::vec4(
                    -(right + left) / (right - left),
                    -(top + bottom) / (top - bottom),
                    -(far + near) / (far - near),
                    1.0f
                );

            }

            this->projection_matrix = glm::mat4(col0, col1, col2, col3);
            this->need_to_update_projection_matrix = false;

        }

    }
}