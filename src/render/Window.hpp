#ifndef CONVEX_HULL_RENDER_WINDOW_HPP
#define CONVEX_HULL_RENDER_WINDOW_HPP

#include <string>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>
#include "scene/Scene.hpp"

namespace convex_hull {
    namespace render {

        class Window {

            private:
                std::size_t width;
                std::size_t height;
                glm::vec4 clear_color;
                std::string title;
                GLFWwindow* glfw_handle;

            public:

                Window(std::size_t width = 500, std::size_t height = 500, glm::vec4 clear_color = glm::vec4(glm::vec3(0.0f), 1.0f), std::string title = "Window");
                virtual ~Window ();

                std::size_t get_width () const;
                void set_width (std::size_t width);

                std::size_t get_height () const;
                void set_height (std::size_t height);

                glm::vec4 get_clear_color () const;
                void set_clear_color (glm::vec4 clear_color);

                std::string get_title () const;
                void set_title (std::string title);

                GLFWwindow* get_glfw_handle() const;

                void activate_context () const;

        };

    }
}

#endif