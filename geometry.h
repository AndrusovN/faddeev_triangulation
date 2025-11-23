#pragma once

#include <iostream>

// basic operations with 3D integral vectors
class Vector3 {
    private:
        int _x, _y, _z;
    
    public:
        Vector3(int x, int y, int z): _x(x), _y(y), _z(z) {}

        Vector3(const Vector3& other): _x(other._x), _y(other._y), _z(other._z) {}

        const Vector3 operator-() const {
            return Vector3(-_x, -_y, -_z);
        }

        Vector3& operator+=(const Vector3& other) {
            _x += other._x;
            _y += other._y;
            _z += other._z;
            return *this;
        }

        Vector3& operator-=(const Vector3& other) {
            return *this += -other;
        }

        Vector3 operator+(const Vector3& other) const {
            auto copy = *this;
            return copy += other;
        }

        Vector3 operator-(const Vector3& other) const {
            return *this + -other;
        }

        int dot(const Vector3& other) const {
            return _x*other._x + _y*other._y + _z*other._z;
        }

        int sqr_len() const {
            return dot(*this);
        }

        int x() const {
            return _x;
        }

        int y() const {
            return _y;
        }
        
        int z() const {
            return _z;
        }

        Vector3 cross(const Vector3& other) const {
            return Vector3(
                _y*other._z - _z*other._y,
                _z*other._x - _x*other._z,
                _x*other._y - _y*other._x
            );
        }

        friend std::ostream& operator << (std::ostream&, const Vector3&);
};

// basic operations with 2D integral vectors
class Vector2 {
    private:
        int _x, _y;
    
    public:
        Vector2(): _x(0), _y(0) {}

        Vector2(int x, int y): _x(x), _y(y) {}

        Vector2(const Vector2& other): _x(other._x), _y(other._y) {}

        const Vector2 operator-() const {
            return Vector2(-_x, -_y);
        }

        Vector2& operator+=(const Vector2& other) {
            _x += other._x;
            _y += other._y;
            return *this;
        }

        Vector2& operator-=(const Vector2& other) {
            return *this += -other;
        }

        Vector2 operator+(const Vector2& other) const {
            auto copy = *this;
            return copy += other;
        }

        Vector2 operator-(const Vector2& other) const {
            return *this + -other;
        }

        int dot(const Vector2& other) const {
            return _x*other._x + _y*other._y;
        }

        int sqr_len() const {
            return dot(*this);
        }

        Vector2 orthogonal() const {
            return Vector2(-_y, _x);
        }

        int x() const {
            return _x;
        }

        int y() const {
            return _y;
        }

        bool operator==(const Vector2& other) const {
            return _x == other._x && _y == other._y;
        }

        friend std::ostream& operator << (std::ostream&, const Vector2&);
};

std::ostream& operator << (std::ostream& o, const Vector3& point) {
    o << "(" << point._x << ", " << point._y << ", " << point._z << ")";
    return o;
}

std::ostream& operator << (std::ostream& o, const Vector2& point) {
    o << "(" << point._x << ", " << point._y << ")";
    return o;
}

// Oriented integral distance from point from to the line through points first and second
int distance_signed(const Vector2& from, const Vector2& first, const Vector2& second) {
    auto diff = second - first;
    auto offset = from - first;
    return offset.dot(diff.orthogonal());
}

// Integral triangle
class Triangle {
    private: 
        Vector2 vertices[3];
    public:
        // counterclockwise
        Triangle(const Vector2& A, const Vector2& B, const Vector2& C) {
            vertices[0] = A;
            vertices[1] = B;
            vertices[2] = C;
        }

        bool is_inside(const Vector2& pt) const {
            if (distance_signed(pt, vertices[0], vertices[1]) <= 0) return false;
            if (distance_signed(pt, vertices[1], vertices[2]) <= 0) return false;
            if (distance_signed(pt, vertices[2], vertices[0]) <= 0) return false;
            return true;
        }
        
        // coordinates of the output are integral distances to each side of the triangle respectively 
        Vector3 get_distances(const Vector2& pt) const {
            return Vector3(
                distance_signed(pt, vertices[0], vertices[1]),
                distance_signed(pt, vertices[1], vertices[2]),
                distance_signed(pt, vertices[2], vertices[0])
            );
        }

        // the height function
        int function_value(const Vector2& pt) const {
            return get_distances(pt).sqr_len();
        }

        friend std::ostream& operator << (std::ostream&, const Triangle&);

        Vector2 get_vertex(int id) const {
            return vertices[id];
        }
};

std::ostream& operator << (std::ostream& o, const Triangle& t) {
    o << "Triangle:\n" << t.vertices[0] << "; " << t.vertices[1] << "; " << t.vertices[2] << std::endl;
    return o;
}
