#ifndef CONVEX_HULL_MATH_VECTOR2_HPP
#define CONVEX_HULL_MATH_VECTOR2_HPP

#include <stdlib.h>

namespace convex_hull {

    namespace math {

        class Vector2 {

            public:

                float x, y;

                Vector2(const float scalar = 0.0);

                Vector2(const float x, const float y);

                float operator [] (const std::size_t index) const;

                float& operator [] (const std::size_t index);

                Vector2 operator + (Vector2 const& vector) const;

                Vector2 operator - (Vector2 const& vector) const;

                float operator * (Vector2 const& vector) const;

                Vector2 operator * (float const& escalar) const;

                float norm() const;

                void normalize();

                Vector2 unit() const;

                float dot(Vector2 const& vector) const;

                float cross(Vector2 const& vector) const;

        };

    }

}

#endif