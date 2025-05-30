#include "render/Shader.hpp"
#include <iostream>
#include <string>
#include <fstream>

namespace convex_hull {
    namespace render {

        Shader::Shader (GLenum _type) : type(_type) {}

        Shader::~Shader () {

            this->destroy();

        }

        GLuint Shader::get_id () const {

            return this->id.value_or(-1);

        }

        GLenum Shader::get_type () const {

            return this->type;

        }

        void Shader::compile (const std::string source_path) {

            // Deletes shader if it is already compiled.
            if (this->id.has_value()) { 

                this->destroy();

            }

            this->id = glCreateShader(this->get_type());

            // Reads source code of the shader from the file.
            std::ifstream stream(source_path);
            if (!stream.is_open()) {

                throw std::runtime_error(std::string("Failed to open file \"") + source_path + "\"!");

            }
            std::string source_code((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
            stream.close();

            // Place the source code in the shader object and compile.
            const char* source_code_pointer = source_code.data();
            glShaderSource(this->get_id(), 1, &source_code_pointer, nullptr);
            glCompileShader(this->get_id());

            // Checks if there is any log message for compilation and print it.
            int log_message_length;
            glGetShaderiv(this->get_id(), GL_INFO_LOG_LENGTH, &log_message_length);
            if (log_message_length > 1) {

                std::string log_message;
                // std::vector<char> log_message(log_message_length);
                log_message.resize(log_message_length);
                glGetShaderInfoLog(this->get_id(), log_message_length, nullptr, log_message.data());
                std::cout << "GLSL compiler (" << source_path << "): " << log_message.data() << std::endl;

            }

            // Checks if the shader compiled correctly.
            GLint compilation_status = GL_FALSE;
            glGetShaderiv(this->get_id(), GL_COMPILE_STATUS, &compilation_status);
            if (!compilation_status) {

                this->destroy();
                throw std::runtime_error(std::string("GLSL compiler (" + source_path + "): Failed to compile shader!"));

            }

        }

        void Shader::destroy () {

            if (this->id.has_value()) glDeleteShader(this->get_id());
            this->id.reset();

        }

    }
}