#include <fstream>
#include "geometry.h"
#include "fadeev_triangles.h"

std::vector<Triangle> triangulate(const Triangle& T) {
    auto inside_points = find_inside_points(T);
    //std::cout << inside_points.size() << std::endl;
    inside_points.push_back(T.get_vertex(0));
    inside_points.push_back(T.get_vertex(1));
    inside_points.push_back(T.get_vertex(2));

    std::vector<Triangle> result;
    for (size_t i = 0; i < inside_points.size() - 3; ++i) {
        Vector3 A = Vector3(inside_points[i].x(), inside_points[i].y(), T.function_value(inside_points[i]));
        for (size_t j = i + 1; j < inside_points.size(); ++j) {
            Vector3 B = Vector3(inside_points[j].x(), inside_points[j].y(), T.function_value(inside_points[j]));
            for (size_t k = i + 1; k < inside_points.size(); ++k) {
                if (j == k) continue;
                Vector3 C = Vector3(inside_points[k].x(), inside_points[k].y(), T.function_value(inside_points[k]));
                auto normal = (B - A).cross(C - A);
                bool positive = true;
                for (auto& pt : inside_points) {
                    Vector3 D = Vector3(pt.x(), pt.y(), T.function_value(pt));
                    if (normal.dot(D - A) < 0) {
                        positive = false;
                        //std::cout << "Failed triangle check:\n" << A << "; " << B << "; " << C << ", " << D << std::endl;
                        break;
                    }
                }
                if (positive) {
                    result.push_back(Triangle(
                        Vector2(A.x(), A.y()), 
                        Vector2(B.x(), B.y()), 
                        Vector2(C.x(), C.y())
                        ));
                }
            }
        }
    }
    return result;
}

void print_point(Vector2 pt, const Triangle& T, std::ofstream& f, int id, const Triangle& sample) {
    Vector3 coordinates = T.get_distances(pt);
    float x = sample.get_vertex(0).x() * coordinates.y() + sample.get_vertex(1).x() * coordinates.z() + sample.get_vertex(2).x() * coordinates.x();
    float y = sample.get_vertex(0).y() * coordinates.y() + sample.get_vertex(1).y() * coordinates.z() + sample.get_vertex(2).y() * coordinates.x();
    f << "z" << id << " = (" << x << "," << y << ");\n";
}

void print_point(Vector2 pt, const Triangle& T, std::ofstream& f, int id) {
    print_point(pt, T, f, id, T);
}

void generate_mp_picture(const std::vector<Triangle>& triangulation, const Triangle& T, const std::string& filename) {
    std::ofstream f(filename, std::ios_base::out);
    f << "prologues:=3;\nverbatimtex\n%&latex\n\\documentclass{minimal}\n\\begin{document}\netex\nbeginfig(0);\n";
    int id = 1;

    Triangle sample(Vector2(-12, 0), Vector2(12, 0), Vector2(0, 18));

    for (auto& triangle : triangulation) {
        print_point(triangle.get_vertex(0), T, f, id, sample);
        print_point(triangle.get_vertex(1), T, f, id + 1, sample);
        print_point(triangle.get_vertex(2), T, f, id + 2, sample);

        f << "draw z" << id << "--z" << id + 1 << "--z" << id + 2 << "--z" << id << ";\n";
        id += 3;
    }

    auto points = find_inside_points(T);
    for (auto& pt : points) {
        print_point(pt, T, f, id++, sample);
        auto distances = T.get_distances(pt);
        f << "label.urt(\"(" << distances.x() << "," << distances.y() << "," << distances.z() << ")\", z" << id - 1 << ");\n";
    }

    f << "endfig;\nend.";
    f.close();
}

int main() {
    //Triangle special = Triangle(Vector2(0,0), Vector2(3, 5), Vector2(-2, 3));
    Triangle special = Triangle(Vector2(0, 0), Vector2(-5, 11), Vector2(-1, 0));
    auto triangulation = triangulate(special);
    generate_mp_picture(triangulation, special, "triangulation11.mp");
}