#pragma once

#include<cassert>
#include<cmath>
#include<string>

// === TUPLE ===
template <typename T>
class Tuple3 {
public:

    Tuple3() : x(T{}), y(T{}), z(T{}) {}
    Tuple3(T x, T y, T z) : x(x), y(y), z(z) { assert(!HasNAN()); }

    // Check for NaN values, only works for floating-point types
    bool HasNAN() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }

    static constexpr int nDimensions = 3;
    T x{}, y{}, z{};
    
    // Accessors
    T operator[](int i) const {
        assert(i >= 0 && i < nDimensions);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    T& operator[](int i) {
        assert(i >= 0 && i < nDimensions);
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    // Arithmetic operations
    Tuple3<T> operator+(const Tuple3<T>& other) const {
        assert(!other.HasNAN());
        return {x + other.x, y + other.y, z + other.z};
    }

    Tuple3<T> operator-(const Tuple3<T>& other) const {
        assert(!other.HasNAN());
        return {x - other.x, y - other.y, z - other.z};
    }

    Tuple3<T> operator*(const T s) const {
        assert(!std::isnan(s));
        return {x * s, y * s, z * s};
    }

    Tuple3<T> operator/(const T d) const {
        assert(d != 0);
        return {x / d, y / d, z / d};
    }

    Tuple3<T> operator-() const {
        return {-x, -y, -z};
    }

    Tuple3<T>& operator+=(const Tuple3<T>& other){
        assert(!other.HasNAN());
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    Tuple3<T>& operator-=(const Tuple3<T>& other){
        assert(!other.HasNAN());
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    Tuple3<T>& operator*=(const T s) {
        assert(!std::isnan(s));
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }

    Tuple3<T>& operator/=(const T d) {
        assert(d != 0);
        x /= d;
        y /= d;
        z /= d;
        return *this;
    }

    // Comparison operators
    bool operator==(const Tuple3<T>& c) const {
        return x == c.x && y == c.y && z == c.z;
    }

    template <typename U>
    bool operator!=(const Tuple3<T>& c) const {
        return !(*this == c);
    }
};

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const Tuple3<T>& t) {
    return out << "(" << t.x << ", " << t.y << ", " << t.z << ")";
}

template <typename T>
inline Tuple3<T> operator*(T s, const Tuple3<T>& t) {
    return t * s;
}

template <typename T>
inline Tuple3<T> Abs(const Tuple3<T>& t) {
    using std::abs;
    return {abs(t.x), abs(t.y), abs(t.z)};
}

template <typename T>
inline Tuple3<T> Ceil(const Tuple3<T>& t) {
    using std::ceil;
    return {ceil(t.x), ceil(t.y), ceil(t.z)};
}

template <typename T>
inline Tuple3<T> Floor(const Tuple3<T>& t) {
    using std::floor;
    return {floor(t.x), floor(t.y), floor(t.z)};
}

template <typename T>
inline Tuple3<T> Lerp(float t, const Tuple3<T>& a, const Tuple3<T>& b){
    return (1 - t) * a + t * b;
}

template <typename T>
inline Tuple3<T> Max(const Tuple3<T>& a, const Tuple3<T>& b) {
    using std::max;
    return {max(a.x, b.x), max(a.y, b.y), max(a.z, b.z)};
}

template <typename T>
inline Tuple3<T> Min(const Tuple3<T>& a, const Tuple3<T>& b) {
    using std::min;
    return {min(a.x, b.x), min(a.y, b.y), min(a.z, b.z)};
}

// === VECTOR ===
template <typename T>
class Vector3 : public Tuple3<T> {
public:

    // Inherit x, y, z from Tuple3
    // Need to use the 'using' keyword since the base class is a template class
    using Tuple3<T>::x;
    using Tuple3<T>::y;
    using Tuple3<T>::z;

    // Default constructor initializes to (0, 0, 0)
    Vector3() : Tuple3<T>() {} 
    
    // Parameterized constructor
    Vector3(T x, T y, T z) : Tuple3<T>(x, y, z) {
        assert(!this->HasNAN());
    }   

    // Conversion constructor, due to the 'explicit' keyword. Converts Tuple3<U> -> Vector3<T> 
    // The template allows conversion from any Tuple3<U> where U can be different from T.
    // EXPLICIT CONVERSION -    Vector3<float> v = Vector3<float>(tuple); 
    // STATIC CAST -            Vector3<float> v = static_cast<Vector3<float>>(tuple);
    // UNIFORM INITIALIZATION - Vector3<float> v{tuple};
    template <typename U>
    explicit Vector3(const Tuple3<U>& t)
        : Tuple3<T>(t.x, t.y, t.z) {
        assert(!this->HasNAN());
    }

    // template <typename U>
    // explicit Vector3(const Point3<U>& p);

    // template <typename U>
    // explicit Vector3(const Normal3<U>& n);

};

using Vec3i = Vector3<int>;
using Vec3f = Vector3<float>;

template <typename T>
inline T LengthSquared(const Vector3<T>& v) {
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

// Type trait 'type' defined here to return the type of the length of a vector based on the vecotr component type
template <typename T>
struct TupleLength { using type = float; };

template <>
struct TupleLength<double> { using type = double; };

// 'auto' tells the compile to specify the return type later
// The trailing return type syntax (->) allows us to use the TupleLength<T>::type based on type T
// Vector3<int>    -> TupleLength<int>::type = Float
// Vector3<float>  -> TupleLength<float>::type = float
// Vector3<double> -> TupleLength<double>::type = double
template <typename T>
inline auto Length(const Vector3<T>& v) -> typename TupleLength<T>::type {
    using std::sqrt;
    return sqrt(LengthSquared(v));
}

template <typename T>
inline auto Normalise(const Vector3<T>& v) {
    return v / Length(v);  // Divide by length to normalize
} 

template <typename T>
inline T Dot(const Vector3<T>& u, const Vector3<T>& v) {
    assert(!u.HasNAN() && !v.HasNAN());
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template <typename T>
inline T AbsDot(const Vector3<T>& u, const Vector3<T>& v) {
    assert(!u.HasNAN() && !v.HasNAN());
    return std::abs(Dot(u, v));
}

// Gram-Schmidt orthogonalization, returns a vector orthogonal to v 
// Note that v is assumed to be normalized
template <typename T>
inline Vector3<T> GramSchmidt(const Vector3<T>& u, const Vector3<T>& v) {
    assert(!u.HasNAN() && !v.HasNAN());
    return u - Dot(u, v) * v;
}

// TODO: Implement difference of products for less loss in accuracy
template <typename T>
inline Vector3<T> Cross(const Vector3<T>& u, const Vector3<T>& v) {
    assert(!u.HasNAN() && !v.HasNAN());
    return Vector3<T>(
        u.y * v.z - u.z * v.y,
        u.z * v.x - u.x * v.z,
        u.x * v.y - u.y * v.x
    );
}

// Angle between two vectors in radians
// template <typename T>
// inline Float Cosine(const Vector3<T>& u, const Vector3<T>& v) {
//     assert(!u.HasNAN() && !v.HasNAN());

//     if (Dot(u, v) < 0) {
//         return PI - 2 * SafeASin(Length(u + v) / 2);
//     }
//     else{
//         return 2 * SafeASin(Length(v - u) / 2);
//     }
// }


// Point needs to be defined differently from Vector, as it represents a point in space rather than a direction or vector.
// point - point = vector
// point + vector = point
// point - vector = point
// vector + vector = vector
// point + point = undefined (not a valid operation)
// === POINT ===
template <typename T>
class Point3 : public Tuple3<T> {
public:
    using Tuple3<T>::x;
    using Tuple3<T>::y;
    using Tuple3<T>::z;

    Point3() = default;
    Point3(T x, T y, T z) : Tuple3<T>(x, y, z) {
        assert(!this->HasNAN());
    }

    template <typename U>
    explicit Point3(const Point3<U>& t)
        : Point3<T>(t.x, t.y, t.z) {}
    
    template <typename U>
    explicit Point3(const Vector3<U>& v)
        : Point3<T>(v.x, v.y, v.z) {}

    // Point + Vector = Point
    template <typename U>
    auto operator+(const Vector3<U> v) const -> Point3<decltype(T{} + U{})> {
        return {x + v.x, y + v.y, z + v.z};
    }

    Point3<T>& operator+=(const Vector3<T>& v) {
        assert(!v.HasNAN());
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    // Point - Vector = Point
    template <typename U>
    auto operator-(const Vector3<U> v) const -> Point3<decltype(T{} - U{})> {
        return {x - v.x, y - v.y, z - v.z};
    }  

    Point3<T>& operator-=(const Vector3<T>& v) {
        assert(!v.HasNAN());
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    // Point - Point = Vector
    template <typename U>
    auto operator-(const Point3<U>& p) const -> Vector3<decltype(T{} - U{})> {
        return {x - p.x, y - p.y, z - p.z};
    }
};

using Point3f = Point3<float>;
using Point3i = Point3<int>;

template <typename T>
inline auto Distance(const Point3<T>& p1, const Point3<T>& p2) -> decltype(Length(p1 - p2)) {
    return Length(p1 - p2);
}

template <typename T>
inline auto DistanceSquared(const Point3<T>& p1, const Point3<T>& p2){
    return LengthSquared(p1 - p2);
}

// === NORMAL ===
template <typename T>
class Normal3 : public Tuple3<T> {
public:
    using Tuple3<T>::x;
    using Tuple3<T>::y;
    using Tuple3<T>::z; 

    Normal3() = default;
    Normal3(T x, T y, T z) : Tuple3<T>(x, y, z) {}

    template <typename U>
    explicit Normal3(const Normal3<U>& t)
        : Tuple3<T>(t.x, t.y, t.z) {}
    
    template <typename U>
    explicit Normal3(const Vector3<U>& t)
        : Tuple3<T>(t.x, t.y, t.z) {}

};

using Normal3f = Normal3<float>;

template <typename T>
Normal3<T> FaceForward(Normal3<T> n, Vector3<T> v) {
    return (Dot(n, v) < 0.f) ? -n : n;
}