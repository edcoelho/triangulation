#include "QuickHull.hpp"
#include <algorithm>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace convex_hull {

    std::vector<glm::vec2> QuickHull::compute_hull (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high) const {

        if (points.size() <= 1) {

            return points;

        }

        glm::vec2
            pivot_vector = pivot_high - pivot_low,
            aux_vector, far_point;

        std::vector<glm::vec2> partition1, partition2, final_hull;

        float
            triangle_area, angle,
            max_triangle_area = 0.0f,
            max_angle = 0.0f;

        // Finding the point with the maximum distance from the line.
        for (std::size_t i = 0; i < points.size(); ++i) {

            aux_vector = points[i] - pivot_low;
            triangle_area = glm::cross(glm::vec3(pivot_vector, 0.0f), glm::vec3(aux_vector, 0.0f)).z/2.0f;

            if (triangle_area > max_triangle_area) {

                max_triangle_area = triangle_area;
                far_point = points[i];

            } else if (triangle_area == max_triangle_area) {

                angle = glm::angle(pivot_vector, aux_vector);

                if (angle > max_angle) {

                    max_angle = angle;
                    far_point = points[i];

                }

            }

        }

        std::tie(partition1, partition2) = this->divide(points, pivot_low, far_point);
        partition2 = this->divide(partition2, far_point, pivot_high).first;

        partition1 = this->compute_hull(partition1, pivot_low, far_point);
        partition2 = this->compute_hull(partition2, far_point, pivot_high);

        // Concatenating left and right hull and initial pivot points.
        final_hull.reserve(partition1.size() + partition2.size() + 1);
        final_hull.insert(final_hull.end(), partition1.begin(), partition1.end());
        final_hull.push_back(far_point);
        final_hull.insert(final_hull.end(), partition2.begin(), partition2.end());

        return final_hull;

    }

    std::pair<std::vector<glm::vec2>, std::vector<glm::vec2>> QuickHull::divide (std::vector<glm::vec2> const& points, glm::vec2 const& pivot_low, glm::vec2 const& pivot_high) const {

        std::pair<std::vector<glm::vec2>, std::vector<glm::vec2>> result;
        float aux;

        result.first.reserve(points.size());
        result.second.reserve(points.size());

        for (std::size_t i = 0; i < points.size(); ++i) {

            aux = glm::cross(glm::vec3(pivot_high - pivot_low, 0.0f), glm::vec3(points[i] - pivot_low, 0.0f)).z;

            if (aux > 0)
            
                result.first.push_back(points[i]);

            else if(aux < 0)

                result.second.push_back(points[i]);

        }

        result.first.shrink_to_fit();
        result.second.shrink_to_fit();

        return result;

    }

    std::vector<glm::vec2> QuickHull::compute_hull (std::vector<glm::vec2> const& points) const {

        if (points.size() > 2) {

            glm::vec2
                pivot_low = points[0],
                pivot_high = points[0];
            std::vector<glm::vec2> left_partition, right_partition;
            std::vector<glm::vec2> result;

            // Finding indices of points with minimum and maximum abscissa.
            for (std::size_t i = 1; i < points.size(); ++i) {

                if (points[i].x < pivot_low.x) pivot_low = points[i];
                if (points[i].x > pivot_high.x) pivot_high = points[i];

            }

            std::tie(left_partition, right_partition) = this->divide(points, pivot_low, pivot_high);

            left_partition = this->compute_hull(left_partition, pivot_low, pivot_high);
            right_partition = this->compute_hull(right_partition, pivot_high, pivot_low);

            // Concatenating left and right hull and initial pivot points.
            result.reserve(left_partition.size() + right_partition.size() + 2);
            result.push_back(pivot_low);
            result.insert(result.end(), left_partition.begin(), left_partition.end());
            result.push_back(pivot_high);
            result.insert(result.end(), right_partition.begin(), right_partition.end());

            return result;

        } else {

            return points;

        }

    }

}