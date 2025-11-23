#include <fstream>
#include "geometry.h"
#include "fadeev_triangles.h"

// Applies the described method to obtain a triangulation of T
// T should be a prime triangle
std::vector<Triangle> triangulate(const Triangle& T) {
    // inside_points are all points that will be used in the triangulation
    auto inside_points = find_inside_points(T);
    inside_points.push_back(T.get_vertex(0));
    inside_points.push_back(T.get_vertex(1));
    inside_points.push_back(T.get_vertex(2));

    std::vector<Triangle> result;
    // iterate over all triplets of points and check if they form a face triangle of Conv(S) 
    for (size_t i = 0; i < inside_points.size() - 3; ++i) {
        Vector3 A = Vector3(inside_points[i].x(), inside_points[i].y(), T.function_value(inside_points[i]));
        for (size_t j = i + 1; j < inside_points.size(); ++j) {
            Vector3 B = Vector3(inside_points[j].x(), inside_points[j].y(), T.function_value(inside_points[j]));
            for (size_t k = i + 1; k < inside_points.size(); ++k) {
                if (j == k) continue;
                Vector3 C = Vector3(inside_points[k].x(), inside_points[k].y(), T.function_value(inside_points[k]));
                auto normal = (B - A).cross(C - A);
                bool positive = true;

                // (A, h(A)),  (B, h(B)), (C, h(C)) form a triangle face in Conv(S) if all other points lie on the one side from this plane
                for (auto& pt : inside_points) {
                    Vector3 D = Vector3(pt.x(), pt.y(), T.function_value(pt));
                    if (normal.dot(D - A) < 0) {
                        positive = false;
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

// Next 3 functions are for producing MetaPost code

// print a point pt with integral distances to the sides of T in the file f
// the point name is z<id>, sample triangle's coordinates correspond to the real coordinates on the page
void print_point(Vector2 pt, const Triangle& T, std::ofstream& f, int id, const Triangle& sample) {
    Vector3 coordinates = T.get_distances(pt);
    float x = sample.get_vertex(0).x() * coordinates.y() + sample.get_vertex(1).x() * coordinates.z() + sample.get_vertex(2).x() * coordinates.x();
    float y = sample.get_vertex(0).y() * coordinates.y() + sample.get_vertex(1).y() * coordinates.z() + sample.get_vertex(2).y() * coordinates.x();
    f << "z" << id << " = (" << x << "," << y << ");\n";
}

void print_point(Vector2 pt, const Triangle& T, std::ofstream& f, int id) {
    print_point(pt, T, f, id, T);
}

// create the MetaPost file
void generate_mp_picture(const std::vector<Triangle>& triangulation, const Triangle& T, const std::string& filename, const char* output_format) {
    std::ofstream f(filename, std::ios_base::out);
    f << "outputformat:=\"";
    f << output_format;
    f << "\";\nprologues:=3;\nverbatimtex\n%&latex\n\\documentclass{minimal}\n\\begin{document}\netex\nbeginfig(0);\n";
    int id = 1;

    Triangle sample(Vector2(-12, 0), Vector2(12, 0), Vector2(0, 18));

    // draw the triangulation lines
    for (auto& triangle : triangulation) {
        print_point(triangle.get_vertex(0), T, f, id, sample);
        print_point(triangle.get_vertex(1), T, f, id + 1, sample);
        print_point(triangle.get_vertex(2), T, f, id + 2, sample);

        f << "draw z" << id << "--z" << id + 1 << "--z" << id + 2 << "--z" << id << ";\n";
        id += 3;
    }

    auto points = find_inside_points(T);
    // print the information about the points
    for (auto& pt : points) {
        print_point(pt, T, f, id++, sample);
        auto distances = T.get_distances(pt);
        f << "label.urt(\"(" << distances.x() << "," << distances.y() << "," << distances.z() << ")\", z" << id - 1 << ");\n";
    }

    f << "endfig;\nend.";
    f.close();
}

int main(int argc, char** argv) {
    const char* output_format = "png";
    if (argc == 2) {
        output_format = argv[1];
    }

    // The triangle from Example 2.1. of Alastair Craw paper
    Triangle special = Triangle(Vector2(0, 0), Vector2(-5, 11), Vector2(-1, 0));
    auto triangulation = triangulate(special);
    generate_mp_picture(triangulation, special, "triangulation.mp", output_format);
}
