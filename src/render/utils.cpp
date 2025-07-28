#include "utils.hpp"
#include <GL/glew.h>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>

namespace triangulation {
    namespace render {

        std::vector<std::vector<glm::vec2>> parse_obj (const std::string& file_name) {

            std::ifstream file(file_name);
            if (!file) {

                throw std::invalid_argument("Failed to open file: " + file_name + "\n");

            }

            std::vector<std::vector<glm::vec2>> groups;
            int current_group = -1;
            std::string line;

            while (std::getline(file, line)) {

                std::istringstream ss(line);
                std::string prefix;
                ss >> prefix;

                if (prefix == "v") {

                    if (current_group == -1) {

                        groups.push_back(std::vector<glm::vec2>());
                        ++current_group;

                    }

                    float x, y;
                    ss >> x >> y;

                    groups[current_group].emplace_back(x, y);

                } else if (prefix == "g") {

                    groups.push_back(std::vector<glm::vec2>());
                    ++current_group;

                }

            }

            return groups;

        }

        std::string get_openGL_version () {

            int major, minor;
            const char* version = (const char*) glGetString(GL_VERSION);

            if (version) {

                return std::string(version);

            } else {

                glGetIntegerv(GL_MAJOR_VERSION, &major);
                glGetIntegerv(GL_MINOR_VERSION, &minor);
                return std::to_string(major) + "." + std::to_string(minor);

            }

        }

        void APIENTRY openGL_debug_message_callback (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {

            std::cout << "\nOpenGL Error: " << std::endl;
            std::cout << "\tOpenGL version: " << get_openGL_version() << std::endl;

            std::cout << "\tError source: ";
            switch (source) {

                case GL_DEBUG_SOURCE_API:
                    std::cout << "API";
                    break;

                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                    std::cout << "Window System";
                    break;

                case GL_DEBUG_SOURCE_SHADER_COMPILER:
                    std::cout << "Shader Compiler";
                    break;

                case GL_DEBUG_SOURCE_THIRD_PARTY:
                    std::cout << "Third Party";
                    break;

                case GL_DEBUG_SOURCE_APPLICATION:
                    std::cout << "Application";
                    break;

                case GL_DEBUG_SOURCE_OTHER:
                    std::cout << "Other";
                    break;

                default:
                    std::cout << "Unknown";
                    break;

            }
            std::cout << std::endl;

            std::cout << "\tError type: ";
            switch (type) {
                case GL_DEBUG_TYPE_ERROR:
                    std::cout << "Error";
                    break;

                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                    std::cout << "Deprecated Behavior";
                    break;

                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                    std::cout << "Undefined Behavior";
                    break;

                case GL_DEBUG_TYPE_PORTABILITY:
                    std::cout << "Portability";
                    break;

                case GL_DEBUG_TYPE_PERFORMANCE:
                    std::cout << "Performance";
                    break;

                case GL_DEBUG_TYPE_OTHER:
                    std::cout << "Other";
                    break;

                default:
                    std::cout << "Unknown";
                    break;

            }
            std::cout << std::endl;

            std::cout << "\tID: " << id << std::endl;

            std::cout << "\tError severity: ";
            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:
                    std::cout << "High";
                    break;

                case GL_DEBUG_SEVERITY_MEDIUM:
                    std::cout << "Medium";
                    break;

                case GL_DEBUG_SEVERITY_LOW:
                    std::cout << "Low";
                    break;

                case GL_DEBUG_SEVERITY_NOTIFICATION:
                    std::cout << "Notification";
                    break;

                default:
                    std::cout << "Unknown";
                    break;

            }
            std::cout << std::endl;

            std::cout << "\tError message: " << message << std::endl << std::endl;

        }

        void enable_openGL_debug_messages (bool enable_notifications) {

            glEnable(GL_DEBUG_OUTPUT);
            glDebugMessageCallback((GLDEBUGPROC) openGL_debug_message_callback, (void*)0);

            if (enable_notifications) {

                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, GL_TRUE);

            } else {

                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, 0, GL_FALSE);

            }

        }

    }
}