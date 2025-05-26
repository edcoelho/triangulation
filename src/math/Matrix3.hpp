#ifndef CONVEX_HULL_MATH_MATRIX3_HPP
#define CONVEX_HULL_MATH_MATRIX3_HPP

#include "math/Vector2.hpp"

namespace convex_hull {

    namespace math {

        class Matrix3 {

            private:

                float data[4][4];

            public:

                Matrix3();
        
                Matrix3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22, float a30, float a31, float a32);

                Matrix3(float scalar);

                float operator () (const std::size_t row, const std::size_t column) const;

                float& operator () (const std::size_t row, const std::size_t column);

                Matrix3 operator + (Matrix3 const& matrix) const;

                Matrix3 operator - (Matrix3 const& matrix) const;

                Matrix3 operator * (Matrix3 const& matrix) const;

                Matrix3 operator * (float const scalar) const;

                Vector2 operator * (Vector2 const& vector) const;

        };

    }

}

#endif