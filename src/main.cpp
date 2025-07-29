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
#include "scene/Camera.hpp"
#include "AdvancingFront.hpp"

using namespace triangulation;

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

bool
    render_groups_triangulations = false,
    render_triangulation = false;

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
        render::Window window(700, 700, glm::vec4(0.0, 0.0, 0.0, 1.0), "Triangulation Visualizer");

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

        std::vector<std::vector<glm::vec2>> vertices_groups;
        if (argc > 1) {

            vertices_groups = render::parse_obj(argv[1]);

        } else {

            // Generating random points.
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(camera.get_left() + (camera.get_right() - camera.get_left())/20.0f, camera.get_right() - (camera.get_right() - camera.get_left())/20.0f);

            vertices_groups.push_back(std::vector<glm::vec2>());

            for (int i = 0; i < 50; ++i) {

                float x = dist(gen);
                float y = dist(gen);
                vertices_groups[0].emplace_back(x, y);

            }

        }

        std::vector<glm::vec2> vertices;
        for (std::size_t i = 0; i < vertices_groups.size(); i++) {

            for (std::size_t j = 0; j < vertices_groups[i].size(); j++) {

                vertices.push_back(vertices_groups[i][j]);

            }

        }

        std::vector<glm::vec2> triangulation = AdvancingFront::compute_triangulation(vertices);

        std::vector<std::vector<glm::vec2>> groups_triangulation;
        for (std::size_t i = 0; i < vertices_groups.size(); i++) {

            groups_triangulation.push_back(AdvancingFront::compute_triangulation(vertices_groups[i]));

        }

        std::vector<GLuint> vao(2 + groups_triangulation.size(), 0);
        std::vector<GLuint> vbo_pos(2 + groups_triangulation.size(), 0);

        for (std::size_t i = 0; i < 2 + groups_triangulation.size(); i++) {
        
            glGenVertexArrays(1, &vao[i]);
            glGenBuffers(1, &vbo_pos[i]);
            setup_vertex_array(vao[i], vbo_pos[i], pos_attrib);

        }

        // Window loop
        while (!glfwWindowShouldClose(window.get_glfw_handle())) {

            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

            glUniform4f(glGetUniformLocation(program.get_id(), "frag_color"), 1.0f, 1.0f, 1.0f, 1.0f);

            glBindVertexArray(vao[0]);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[0]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
            glPointSize(5);
            glDrawArrays(GL_POINTS, 0, vertices.size());

            if (render_triangulation) {

                glBindVertexArray(vao[1]);
                glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[1]);
                glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*triangulation.size(), triangulation.data(), GL_STATIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, triangulation.size());

            }

            if (render_groups_triangulations) {

                for (std::size_t i = 0; i < groups_triangulation.size(); i++) {

                    glBindVertexArray(vao[i + 2]);
                    glBindBuffer(GL_ARRAY_BUFFER, vbo_pos[i + 2]);
                    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*groups_triangulation[i].size(), groups_triangulation[i].data(), GL_STATIC_DRAW);
                    glDrawArrays(GL_TRIANGLES, 0, groups_triangulation[i].size());

                }

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

        } else if (key == GLFW_KEY_1) {

            render_triangulation = !render_triangulation;

        } else if (key == GLFW_KEY_2) {

            render_groups_triangulations = !render_groups_triangulations;

        }

    }

}

void render::glfw_framebuffer_size_callback (GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);

}