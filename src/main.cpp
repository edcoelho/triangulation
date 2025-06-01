#include <iostream>
#include <stdexcept>
#include <random>

#include <GL/glew.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/Window.hpp"
#include "render/Shader.hpp"
#include "render/Program.hpp"
#include "render/utils.hpp"
#include "AnimationFrame.hpp"
#include "scene/Camera.hpp"
#include "QuickHull.hpp"

using namespace convex_hull;

const std::string path_to_project = "src";

scene::Camera camera(
    glm::vec3(0.0f, 0.0f, 1.0f), // position
    glm::vec3(0.0f, 0.0f, -1.0f), // look_at
    glm::vec3(0.0f, 1.0f, 0.0f), // view_up (in view/camera space)
    scene::ORTHOGRAPHIC, // projection type
    // near and far distances (positive numbers)
    0.01f, 100.0f,
    // bottom, top, left, right
    0.0f, 500.0f, 0.0f, 500.0f
);

render::Program program;
render::Shader vertex_shader(GL_VERTEX_SHADER);
render::Shader fragment_shader(GL_FRAGMENT_SHADER);

glm::mat4
    model_mat(1.0f), // Model transformation
    view_mat = camera.get_view_matrix(), // View transformation
    projection_mat = camera.get_projection_matrix(); // Projection transformation

QuickHull quickhull;

bool
    render_animation = false,
    render_convex_hull = false;
std::size_t animation_frame = 0;
double animation_timer = 0.0;

void setup_vertex_array(GLuint vao, GLuint vbo, GLuint attrib_location);

int main(int argc, char * argv[]) {

    try {

        // Setting GLFW error callback function.
        glfwSetErrorCallback(render::glfw_error_callback);

        // Initializing GLFW.
        if (!glfwInit()) throw std::runtime_error("GLFW: Failed to initialize!");

        // Setting GLFW hints.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        render::Window window(700, 700, glm::vec4(0.0, 0.0, 0.0, 1.0), "Quick Hull Visualizer");

        // Setting GLFW callbacks.
        glfwSetKeyCallback(window.get_glfw_handle(), render::glfw_key_callback);
        glfwSetFramebufferSizeCallback(window.get_glfw_handle(), render::glfw_framebuffer_size_callback);

        window.activate_context();
        glfwSwapInterval(1);

        // Initializing GLEW to load OpenGL API.
        GLenum glew_status = glewInit();
        if (glew_status != GLEW_OK) {

            std::cout << "GLEW: Failed to initialize!" << std::endl;
            std::cout << "GLEW: " << glewGetErrorString(glew_status) << std::endl;
            glfwTerminate();
            return EXIT_FAILURE;

        }

        // Configuring OpenGL resources.
        // glEnable(GL_DEPTH_TEST);
        // glDepthFunc(GL_LESS);

        render::enable_openGL_debug_messages(false);

        program.create();

        vertex_shader.compile(path_to_project + "/shaders/main.vert");
        program.attach(vertex_shader);

        fragment_shader.compile(path_to_project + "/shaders/main.frag");
        program.attach(fragment_shader);

        program.link();
        program.use();

        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "model_mat"), 1, GL_FALSE, glm::value_ptr(model_mat));
        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "view_mat"), 1, GL_FALSE, glm::value_ptr(view_mat));
        glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "projection_mat"), 1, GL_FALSE, glm::value_ptr(projection_mat));

        GLuint pos_attrib = glGetAttribLocation(program.get_id(), "pos");

        std::vector<glm::vec2> vertices;
        if (argc > 1) {

            vertices = render::parse_obj(argv[1]);

        } else {

            // Generating random points.
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(camera.get_left() + (camera.get_right() - camera.get_left())/20.0f, camera.get_right() - (camera.get_right() - camera.get_left())/20.0f);

            for (int i = 0; i < 50; ++i) {

                float x = dist(gen);
                float y = dist(gen);
                vertices.emplace_back(x, y);

            }

        }

        QuickHull::QuickHullResult quickhull_result = quickhull.compute_hull(vertices);

        // Creating VAOs for the static drawing.

        GLuint vao_static[2];
        glGenVertexArrays(2, vao_static);

        // Creating VBOs for vertices and the convex hull.

        GLuint vbo_static_pos[2];
        glGenBuffers(2, vbo_static_pos);

        setup_vertex_array(vao_static[0], vbo_static_pos[0], pos_attrib);
        setup_vertex_array(vao_static[1], vbo_static_pos[1], pos_attrib);

        // Creating VAOs for the animation.

        GLuint vao_animation[2];
        glGenVertexArrays(2, vao_animation);

        // Creating VBOs for a animation frame.

        GLuint vbo_animation_pos[2];
        glGenBuffers(2, vbo_animation_pos);

        setup_vertex_array(vao_animation[0], vbo_animation_pos[0], pos_attrib);
        setup_vertex_array(vao_animation[1], vbo_animation_pos[1], pos_attrib);

        animation_timer = glfwGetTime();

        // Window loop
        while (!glfwWindowShouldClose(window.get_glfw_handle())) {

            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);

            glUniform4f(glGetUniformLocation(program.get_id(), "frag_color"), 1.0f, 1.0f, 1.0f, 1.0f);
            
            glBindVertexArray(vao_static[0]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_static_pos[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
            glDrawArrays(GL_POINTS, 0, vertices.size());

            if (render_animation && quickhull_result.frames.size() > 0) {

                glUniform4f(glGetUniformLocation(program.get_id(), "frag_color"), 1.0f, 0.0f, 1.0f, 1.0f);
                glBindVertexArray(vao_animation[0]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_animation_pos[0]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*quickhull_result.frames[animation_frame].pivot_edges.size(), quickhull_result.frames[animation_frame].pivot_edges.data(), GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, quickhull_result.frames[animation_frame].pivot_edges.size());

                glUniform4f(glGetUniformLocation(program.get_id(), "frag_color"), 1.0f, 1.0f, 1.0f, 1.0f);
                glBindVertexArray(vao_animation[1]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_animation_pos[1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*quickhull_result.frames[animation_frame].hull_edges.size(), quickhull_result.frames[animation_frame].hull_edges.data(), GL_DYNAMIC_DRAW);
                glDrawArrays(GL_LINES, 0, quickhull_result.frames[animation_frame].hull_edges.size());

                if (glfwGetTime() - animation_timer >= 0.8) {

                    animation_timer = glfwGetTime();
                    if (animation_frame + 1 == quickhull_result.frames.size()) {

                        animation_frame = 0;
                        render_animation = false;
                        render_convex_hull = true;

                    } else {

                        animation_frame = animation_frame + 1;

                    }

                }

            } else if (render_convex_hull) {

                glBindVertexArray(vao_static[1]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_static_pos[1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*quickhull_result.vertices.size(), quickhull_result.vertices.data(), GL_STATIC_DRAW);
                glDrawArrays(GL_LINE_LOOP, 0, quickhull_result.vertices.size());

            }

            glfwSwapBuffers(window.get_glfw_handle());
            glfwPollEvents();

        }

        glfwDestroyWindow(window.get_glfw_handle());
        glfwTerminate();

    } catch (const std::exception& e) {

        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;

    }
    return EXIT_SUCCESS;

}

void setup_vertex_array(GLuint vao, GLuint vbo, GLuint attrib_location) {

    glBindVertexArray(vao);
    glEnableVertexAttribArray(attrib_location);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(attrib_location, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

}

void render::glfw_error_callback(int error, const char* description) {

    std::cout << " Error " << error << std::endl;
    std::cout << "GLFW: " << description << std::endl;

}

void render::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (action == GLFW_PRESS) {

        if (key == GLFW_KEY_ESCAPE) {

            glfwSetWindowShouldClose(window, GLFW_TRUE);

        } else if (key == GLFW_KEY_P) {

            render_animation = !render_animation;
            animation_frame = 0;
            animation_timer = glfwGetTime();

        } else if (key == GLFW_KEY_O) {

            render_convex_hull = !render_convex_hull;

        }

    }

}

void render::glfw_framebuffer_size_callback (GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);

}