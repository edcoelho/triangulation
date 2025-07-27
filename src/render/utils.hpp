#ifndef TRIANGULATION_RENDER_UTILS_HPP_
#define TRIANGULATION_RENDER_UTILS_HPP_

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <string>
#include <vector>

#ifndef APIENTRY
#define APIENTRY
#endif

namespace triangulation {
    namespace render {

        std::vector<std::vector<glm::vec2>> parse_obj (const std::string& file_name);

        // OpenGL Debug

        std::string get_openGL_version ();
        void APIENTRY openGL_debug_message_callback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
        void enable_openGL_debug_messages (bool enable_notifications = true);

        // GLFW callbacks (must be implemented in the same file of main function)

        void glfw_error_callback (int error, const char* description);
        void glfw_key_callback (GLFWwindow* window, int key, int scancode, int action, int mods);
        void glfw_framebuffer_size_callback (GLFWwindow* window, int width, int height);

    }
}

#endif