#ifndef CONVEX_HULL_RENDER_SHADER_HPP
#define CONVEX_HULL_RENDER_SHADER_HPP

#include <GL/glew.h>
#include <optional>
#include <string>

namespace convex_hull {
    namespace render {

        class Shader {

            private:
                std::optional<GLuint> id;
                GLenum type;

            public:

                Shader (GLenum _type = GL_VERTEX_SHADER);
                virtual ~Shader ();

                GLuint get_id () const;
                GLenum get_type () const;

                // Compiles the shader from a file.
                void compile (const std::string source_path);
                // Deletes the shader.
                void destroy ();

        };

    }
}

#endif