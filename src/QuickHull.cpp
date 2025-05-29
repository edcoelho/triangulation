#include "QuickHull.hpp"
#include <algorithm>
#include <tuple>

namespace convex_hull {

    std::vector<math::Vector2> QuickHull::compute_hull (std::vector<math::Vector2> const& points, math::Vector2 const& pivot_low, math::Vector2 const& pivot_high) const {

        if (points.size() <= 1) return points;

        math::Vector2
            pivot_vector = pivot_high - pivot_low,
            aux_vector, far_point;

        std::vector<math::Vector2> partition1, partition2, final_hull;

        float
            triangle_area, angle,
            max_triangle_area = 0.0f,
            max_angle = 0.0f;

        // Finding the point with the maximum distance from the line.
        for (std::size_t i = 0; i < points.size(); ++i) {

            aux_vector = points[i] - pivot_low;
            triangle_area = pivot_vector.cross(aux_vector)/2.0f;

            if (triangle_area > max_triangle_area) {

                max_triangle_area = triangle_area;
                far_point = points[i];

            } else if (triangle_area == max_triangle_area) {

                angle = pivot_vector.angle(aux_vector);

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
        final_hull.reserve(partition1.size() + partition1.size() + 1);
        final_hull.insert(final_hull.end(), partition1.begin(), partition1.end());
        final_hull.push_back(far_point);
        final_hull.insert(final_hull.end(), partition2.begin(), partition2.end());

        return final_hull;

    }

    std::pair<std::vector<math::Vector2>, std::vector<math::Vector2>> QuickHull::divide (std::vector<math::Vector2> const& points, math::Vector2 const& pivot_low, math::Vector2 const& pivot_high) const {

        std::pair<std::vector<math::Vector2>, std::vector<math::Vector2>> result;
        float aux;

        result.first.reserve(points.size());
        result.second.reserve(points.size());

        for (std::size_t i = 0; i < points.size(); ++i) {

            aux = (pivot_high - pivot_low).cross(points[i] - pivot_low);

            if (aux > 0)
            
                result.first.push_back(points[i]);

            else if(aux < 0)

                result.second.push_back(points[i]);

        }

        result.first.shrink_to_fit();
        result.second.shrink_to_fit();

        return result;

    }

    std::vector<math::Vector2> QuickHull::compute_hull (std::vector<math::Vector2> const& points) const {

        if (points.size() > 2) {

            math::Vector2
                pivot_low = points[0],
                pivot_high = points[0];
            std::vector<math::Vector2> left_partition, right_partition, final_hull;

            // Finding indices of points with minimum and maximum abscissa.
            for (std::size_t i = 1; i < points.size(); ++i) {

                if (points[i].x < pivot_low.x) pivot_low = points[i];
                if (points[i].x > pivot_high.x) pivot_high = points[i];

            }

            std::tie(left_partition, right_partition) = this->divide(points, pivot_low, pivot_high);

            left_partition = this->compute_hull(left_partition, pivot_low, pivot_high);
            right_partition = this->compute_hull(right_partition, pivot_high, pivot_low);

            // Concatenating left and right hull and initial pivot points.
            final_hull.reserve(left_partition.size() + right_partition.size() + 2);
            final_hull.push_back(pivot_low);
            final_hull.insert(final_hull.end(), left_partition.begin(), left_partition.end());
            final_hull.push_back(pivot_high);
            final_hull.insert(final_hull.end(), right_partition.begin(), right_partition.end());

            return final_hull;

        } else {

            return points;

        }

    }

}