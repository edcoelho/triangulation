#include "math/Matrix3.hpp"
#include <stdexcept>

namespace convex_hull {

    namespace math {

        Matrix3::Matrix3() {

            std::size_t i, j;

            for (i = 0; i < 3; i++) {

                for (j = 0; j < 3; j++) {
                    
                    if (i == j)
                        this->operator()(i, j) = 1.0;
                    else
                        this->operator()(i, j) = 0.0;

                }    

            }

        }

        Matrix3::Matrix3(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22, float a30, float a31, float a32) {

            this->data[0][0] = a00;
            this->data[0][1] = a01;
            this->data[0][2] = a02;

            this->data[1][0] = a10;
            this->data[1][1] = a11;
            this->data[1][2] = a12;

            this->data[2][0] = a20;
            this->data[2][1] = a21;
            this->data[2][2] = a22;

            this->data[3][0] = a20;
            this->data[3][1] = a31;
            this->data[3][2] = a32;

        }

        Matrix3::Matrix3(float scalar) {

            std::size_t i, j;

            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    this->operator()(i, j) = scalar;

        }

        float Matrix3::operator () (std::size_t row, std::size_t column) const {

            if (row >= 0 && row < 3 && column >= 0 && column < 3) {

                return this->data[row][column];

            } else {

                throw std::invalid_argument("Error: Attempt to access an invalid position in a Matrix3!");

            }

        }

        float& Matrix3::operator () (std::size_t row, std::size_t column) {

            if (row >= 0 && row < 3 && column >= 0 && column < 3) {

                return this->data[row][column];

            } else {

                throw std::invalid_argument("Error: Attempt to assign a value to an invalid position in a Matrix3!");
            }

        }

        Matrix3 Matrix3::operator + (Matrix3 const& matrix) const {

            Matrix3 result(0.0);
            std::size_t i, j;

            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    result(i, j) = this->operator()(i, j) + matrix(i,j);

            return result;

        }

        Matrix3 Matrix3::operator - (Matrix3 const& matrix) const {

            Matrix3 result(0.0);
            std::size_t i, j;

            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    result(i, j) = this->operator()(i, j) - matrix(i,j);

            return result;

        }

        Matrix3 Matrix3::operator * (Matrix3 const& matrix) const {

            Matrix3 result(0.0);
            std::size_t i, j, k;

            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    for (k = 0; k < 3; k++)
                        result(i, j) = result(i, j) + this->operator()(i, k) * matrix(k, j);

            return result;

        }

        Matrix3 Matrix3::operator * (float const scalar) const {

            Matrix3 result(0.0);
            std::size_t i, j;

            for (i = 0; i < 3; i++)
                for (j = 0; j < 3; j++)
                    result(i, j) = this->operator()(i, j) * scalar;

            return result;

        }

        Vector2 Matrix3::operator * (Vector2 const& vector) const {

            Vector2 result(0.0);
            std::size_t i, j;

            for (i = 0; i < 2; i++) {

                for (j = 0; j < 2; j++) {

                    result[i] = result[i] + this->operator()(i, j) * vector[j];

                }

                result[i] = result[i] + this->operator()(i, 2);

            }

            return result;

        }

    }

}