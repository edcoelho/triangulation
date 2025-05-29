#include "math/Vector2.hpp"
#include <stdexcept>
#include <cmath>

namespace convex_hull {

    namespace math {

        Vector2::Vector2(const float scalar) {

            this->x = scalar;
            this->y = scalar;

        }

        Vector2::Vector2(const float x, const float y) {

            this->x = x;
            this->y = y;

        }

        float Vector2::operator [] (const std::size_t index) const {

            if (index >= 0 && index < 2) {

                if (index == 0) {

                    return this->x;

                } else {

                    return this->y;

                }

            } else {

                throw std::invalid_argument("Error: Attempt to access an invalid index of a Vector2.");

            }

        }

        float& Vector2::operator [] (const std::size_t index) {

            if (index >= 0 && index < 2) {

                if (index == 0) {

                    return this->x;

                } else {

                    return this->y;

                }

            } else {

                throw std::invalid_argument("Error: Attempt to assign a value to an invalid index of a Vector2.");

            }

        }

        Vector2 Vector2::operator + (Vector2 const& vector) const {

            Vector2 result(0.0);
            std::size_t i;

            for (i = 0; i < 2; i++)
                result[i] = this->operator[](i) + vector[i];

            return result;

        }

        Vector2 Vector2::operator - (Vector2 const& vector) const {

            Vector2 result(0.0);
            std::size_t i;

            for (i = 0; i < 2; i++)
                result[i] = this->operator[](i) - vector[i];

            return result;

        }

        float Vector2::operator * (Vector2 const& vector) const {

            return this->cross(vector);

        }

        Vector2 Vector2::operator * (float const& scalar) const {

            Vector2 result(0.0);
            std::size_t i;

            for (i = 0; i < 2; i++)
                result[i] = this->operator[](i) * scalar;

            return result;

        }

        float Vector2::norm() const {

            float norm = 0.0;
            std::size_t i;

            for (i = 0; i < 2; i++)
                norm = norm + this->operator[](i) * this->operator[](i);

            return std::sqrt(norm);

        }

        void Vector2::normalize() {

            float norm = this->norm();
            std::size_t i;

            for (i = 0; i < 2; i++)
                this->operator[](i) = this->operator[](i)/norm;

        }

        Vector2 Vector2::unit() const {

            Vector2 vector(this->x, this->y);
            float norm = vector.norm();
            std::size_t i;

            for (i = 0; i < 2; i++)
                vector[i] = vector[i]/norm;

            return vector;

        };

        float Vector2::dot(Vector2 const& vector) const {

            float result = 0.0;
            std::size_t i;

            for (i = 0; i < 2; i++)
                result = result + this->operator[](i) * vector[i];

            return result;

        }

        float Vector2::cross(Vector2 const& vector) const {

            return this->x*vector.y - vector.x*this->y;

        }

        float Vector2::angle (Vector2 const& vector) const {

            return std::acos(this->unit().dot(vector.unit()));

        }

    }

}