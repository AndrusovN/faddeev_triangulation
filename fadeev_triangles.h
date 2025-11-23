#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "geometry.h"
#include "auxiliary.h"

// returns all internal integral points of a triangle T
std::vector<Vector2> find_inside_points(const Triangle& T) {
    int min_x = min(T.get_vertex(0).x(), T.get_vertex(1).x(), T.get_vertex(2).x());
    int min_y = min(T.get_vertex(0).y(), T.get_vertex(1).y(), T.get_vertex(2).y());

    int max_x = max(T.get_vertex(0).x(), T.get_vertex(1).x(), T.get_vertex(2).x());
    int max_y = max(T.get_vertex(0).y(), T.get_vertex(1).y(), T.get_vertex(2).y());

    std::vector<Vector2> points_inside;

    for (int x = min_x; x <= max_x; ++x) {
        for (int y = min_y; y <= max_y; ++y) {
            auto pt = Vector2(x, y);
            if (T.is_inside(pt)) {
                points_inside.push_back(pt);
            }
        }
    }

    return points_inside;
}

// for a convex polygon with vertices array polygon checks if point pt is internal for it
bool is_inside_polygon(const Vector2& pt, const std::vector<Vector2>& polygon) {
    int checker = distance_signed(pt, polygon.back(), polygon.front());
    for (int id = 2; id < polygon.size(); ++id) {
        int dist = distance_signed(pt, polygon[id - 1], polygon[id]);
        if (dist * checker < 0) return false;
    }
    return true;
}

// checks if the method described in the article fails for triangle T
// returns 4 points lying on one face of Conv(S) if the method fails
// otherwise returns empty vector
std::vector<Vector2> check_triangle(const Triangle& T) {
    auto points = find_inside_points(T);

    // by h(A)+h(B) returns vector of such pairs (A, B)
    std::map<int, std::vector<std::pair<Vector2, Vector2>>> valued_pairs;
    // building valued_pairs
    for (auto& A : points) {
        for (auto& B : points) {
            if (B == A) continue;
            int a = T.function_value(A);
            int b = T.function_value(B);
            if (valued_pairs.find(a + b) == valued_pairs.end()) {
                valued_pairs[a + b] = std::vector<std::pair<Vector2, Vector2>>({std::make_pair(A, B)});
            } else {
                valued_pairs[a + b].push_back(std::make_pair(A, B));
            }
        }
    }

    // checking if valued_pairs contain non-trivial ones
    for (auto& pair_candidates : valued_pairs) {
        auto& candidates = pair_candidates.second;
        for (auto& first_pair : candidates) {
            for (auto& second_pair : candidates) {
                auto A = first_pair.first;
                auto D = first_pair.second;
                auto C = second_pair.first;
                auto B = second_pair.second;
                if (A == B) continue;
                if (A == C) continue;
                if (D == B) continue;
                if (D == C) continue;

                if (C - A == D - B && B - A == D - C) {
                    bool failed_by_inside_point = false;
                    return {A, B, C, D};
                }
            }
        }
    } 
    return std::vector<Vector2>();
}

// In the case of fail prints information about the fail
void print_full_log(const Triangle& T, int p, std::vector<Vector2>& res) {
    std::cout << "Found bad triangle with p = " << p << std::endl;
    std::cout << "FYI p mod 6 is " << p%6 << std::endl << std::endl;
    std::cout << T << std::endl;
    for (auto& pt : res) {
        std::cout << pt << " with heights " << T.get_distances(pt) << " and function value " << T.function_value(pt) << '\n';
    }
}

// For a given prime p checks if all prime triangles of area p are triangulated properly with the given method
bool check_prime(int p, bool full_log=false) {

    if (!is_prime(p)) return false;
    bool found = false;
    for (int a = 2; a < p; ++a) {
        Triangle T(Vector2(0,0), Vector2(1, 0), Vector2(a, p));
        auto res = check_triangle(T);
        if (!res.empty()) {
            if (p % 6 != 1) {
                std::cout << "Hypothesis failed!!!" << std::endl;
                print_full_log(T, p, res);
            }
            if (inverse(a, p) != 1 + inverse(a - 1, p)) {
                std::cout << "Hypothesis failed!!!" << std::endl;
                print_full_log(T, p, res);
            }
            found = true;
            if (full_log) {
                print_full_log(T, p, res);
                return true;
            }
        }
    }
    std::cout << "Checked p = " << p << std::endl;
    if (found) return found;
    return false;
}
