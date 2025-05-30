#include "render/Window.hpp"
#include <stdexcept>

namespace convex_hull {
    namespace render {

        Window::Window(std::size_t width, std::size_t height, glm::vec4 clear_color, std::string title) : width(width), height(height), clear_color(clear_color), title(title) {

            this->glfw_handle = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            if (!glfw_handle) throw std::runtime_error("GLFW: Window failed to initialize!");
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

        }

        Window::~Window () {

            glfwDestroyWindow(this->glfw_handle);

        }

        std::size_t Window::get_width() const {

            return this->width;

        }
        void Window::set_width(std::size_t width) {

            this->width = width;
            glfwSetWindowSize(this->glfw_handle, width, this->height);

        }

        std::size_t Window::get_height() const {

            return this->height;

        }
        void Window::set_height(std::size_t height) {

            this->height = height;
            glfwSetWindowSize(this->glfw_handle, this->width, height);

        }

        glm::vec4 Window::get_clear_color() const {

            return this->clear_color;

        }
        void Window::set_clear_color(glm::vec4 clear_color) {

            this->clear_color = clear_color;
            glClearColor(clear_color.r, clear_color.g, clear_color.b, clear_color.a);

        }

        std::string Window::get_title() const {

            return this->title;

        }
        void Window::set_title(std::string title) {

            this->title = title;
            glfwSetWindowTitle(this->glfw_handle, title.c_str());

        }

        GLFWwindow* Window::get_glfw_handle() const {

            return this->glfw_handle;

        }

        void Window::activate_context () const {

            glfwMakeContextCurrent(this->glfw_handle);

        }

    }
}