#include <iostream>
#include <stdexcept>

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
    -5.0f, 5.0f, -5.0f, 5.0f
);

render::Program program;
render::Shader vertex_shader(GL_VERTEX_SHADER);
render::Shader fragment_shader(GL_FRAGMENT_SHADER);

glm::mat4
    model_mat(1.0f), // Model transformation
    view_mat = camera.get_view_matrix(), // View transformation
    projection_mat = camera.get_projection_matrix(); // Projection transformation

QuickHull quickhull;

int main(int argc, char * argv[]) {

    try {

        // Setting GLFW error callback function.
        glfwSetErrorCallback(render::glfw_error_callback);

        // Initializing GLFW.
        if (!glfwInit()) throw std::runtime_error("GLFW: Failed to initialize!");

        // Creating window with hits.
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        render::Window window(700, 700, glm::vec4(0.0, 0.0, 0.0, 1.0), "Convex Hull Visualizer");

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

        // Creating a Vertex Array Object (VAO).
        GLuint vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // Creating a Vertex Buffer Object (VBO) for vertex position.
        std::vector<glm::vec2>
            points = {{0, 3}, {1, 1}, {2, 3}, {3, 1}, {2, 2}, {1, 0}},
            convex_hull_points = quickhull.compute_hull(points);
        std::vector<float> vertices_pos;

        vertices_pos.reserve(2*(points.size() + vertices_pos.size()));
        for (auto it = points.begin(); it != points.end(); ++it) {

            vertices_pos.push_back(it->x);
            vertices_pos.push_back(it->y);

        }
        for (auto it = convex_hull_points.begin(); it != convex_hull_points.end(); ++it) {

            vertices_pos.push_back(it->x);
            vertices_pos.push_back(it->y);

        }
        
        GLuint vbo_pos;
        glGenBuffers(1, &vbo_pos);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_pos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertices_pos.size(), vertices_pos.data(), GL_DYNAMIC_DRAW);

        GLuint position_attrib = glGetAttribLocation(program.get_id(), "pos");
        glEnableVertexAttribArray(position_attrib);
        glVertexAttribPointer(position_attrib, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

        // points.clear();
        // points.shrink_to_fit();
        // convex_hull_points.clear();
        // convex_hull_points.shrink_to_fit();
        // vertices_pos.clear();
        // vertices_pos.shrink_to_fit();

        // Window loop
        while (!glfwWindowShouldClose(window.get_glfw_handle())) {

            // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);

            view_mat = camera.get_view_matrix();
            projection_mat = camera.get_projection_matrix();
            glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "view_mat"), 1, GL_FALSE, glm::value_ptr(view_mat));
            glUniformMatrix4fv(glGetUniformLocation(program.get_id(), "projection_mat"), 1, GL_FALSE, glm::value_ptr(projection_mat));

            // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_POINTS, 0, points.size());
            glDrawArrays(GL_LINE_LOOP, points.size(), convex_hull_points.size());

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

void render::glfw_error_callback(int error, const char* description) {

    std::cout << " Error " << error << std::endl;
    std::cout << "GLFW: " << description << std::endl;

}

void render::glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);

}

void render::glfw_framebuffer_size_callback (GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height);

}