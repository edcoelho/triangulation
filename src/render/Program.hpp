#ifndef CONVEX_HULL_RENDER_PROGRAM_HPP_
#define CONVEX_HULL_RENDER_PROGRAM_HPP_

#include <GL/glew.h>
#include "render/Shader.hpp"
#include <optional>
#include <vector>

namespace convex_hull {
    namespace render {

        class Program {

            private:

                std::optional<GLuint> id;

            public:

                Program ();
                ~Program ();

                GLuint get_id () const;

                // Create the program.
                void create ();

                // Attachs a shader to the program.
                void attach (GLuint shader_id);
                // Attachs a shader to the program.
                void attach (Shader shader);

                // Detachs a shader from the program.
                // It is a good idea to detach all shaders objects after linking it.
                void detach (GLuint shader_id);
                // Detachs a shader from the program.
                // It is a good idea to detach all shaders objects after linking it.
                void detach (Shader shader);

                // Links the program.
                // The shaders must be attached before calling this.
                void link();

                // Bind the program to be used.
                void use() const;

                // Deletes the program.
                void destroy ();

        };

    }
}

#endif