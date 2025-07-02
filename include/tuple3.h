#pragma once

#include<cassert>
#include<cmath>
#include<string>

#ifdef DEBUG
    #define DCHECK(condition) assert(condition)
    #define DCHECK_NE(a, b) assert((a) != (b))
#else
    #define DCHECK(condition) // No-op in release builds
    #define DCHECK_NE(a, b)   
#endif


template <template <typename> class Child, typename T>
class Tuple3 {
public:
    Tuple3(T x, T y, T z) : x(x), y(y), z(z) { DCHECK(!HasNAN()); }
    bool HasNAN() const { return std::isnan(x) || std::isnan(y) || std::isnan(z); }
    static constexpr int nDimensions = 3;

    // Accessors
    T operator[](int i) const {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    T& operator[](int i) {
        if (i == 0) return x;
        if (i == 1) return y;
        return z;
    }

    // Arithmetic operations
    template <typename U>
    auto operator+(const Child<U>& c) const -> Child<decltype(T{} + U{})> {
        return {x + c.x, y + c.y, z + c.z};
    }

    template <typename U>
    Child<T>& operator+=(const Child<U>& c) {
        DCHECK(!c.HasNAN());
        x += c.x;
        y += c.y;
        z += c.z;
        return static_cast<Child<T>&>(*this);
    }

    template <typename U>
    auto operator-(const Child<U>& c) const -> Child<decltype(T{} - U{})> {
        DCHECK(!c.HasNAN());
        return {x - c.x, y - c.y, z - c.z};
    }

    template <typename U>
    Child<T> &operator-=(const Child<U>& c) {
        DCHECK(!c.HasNAN());
        x -= c.x;
        y -= c.y;
        z -= c.z;
        return static_cast<Child<T> &>(*this);
    }

    template <typename U>
    auto operator*(U s) const -> Child<decltype(T{} * U{})> {
        return {x * s, y * s, z * s};
    }

    template <typename U>
    Child<T> &operator*=(U s) {
        DCHECK(!std::isnan(s));
        x *= s;
        y *= s;
        z *= s;
        return static_cast<Child<T> &>(*this);
    }

    template <typename U>
    auto operator/(U d) const -> Child<decltype(T{} / U{})> {
        DCHECK_NE(d, 0);
        return {x / d, y / d, z / d};
    }

    template <typename U>
    Child<T> &operator/=(U d) {
        DCHECK_NE(d, 0);
        x /= d;
        y /= d;
        z /= d;
        return static_cast<Child<T> &>(*this);
    }

    Child<T> operator-() const {
        return {-x, -y, -z};
    }

    // Comparison operators
    template <typename U>
    bool operator==(const Child<U>& c) const {
        return x == c.x && y == c.y && z == c.z;
    }

    template <typename U>
    bool operator!=(const Child<U>& c) const {
        return !(*this == c); 
    }

    std::string ToString() const {
        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";
    }

    T x{}, y{}, z{};
};

// template <template <class> class C, typename T, typename U>
// inline auto operator*(U s, Tuple3<C, T> t) -> C<decltype(T{} * U{})> {
//     return t * s;
// }

// template <template <class> class C, typename T>
// inline C<T> Abs(Tuple3<C, T> t) {
//     using std::abs;
//     return {abs(t.x), abs(t.y), abs(t.z)};
// }

// template <template <class> class C, typename T>
// inline C<T> Ceil(Tuple3<C, T> t) {
//     using pstd::ceil;
//     return {ceil(t.x), ceil(t.y), ceil(t.z)};
// }

// template <template <class> class C, typename T>
// inline C<T> Floor(Tuple3<C, T> t) {
//     using pstd::floor;
//     return {floor(t.x), floor(t.y), floor(t.z)};
// }

// template <template <class> class C, typename T>
// inline auto Lerp(Float t, Tuple3<C, T> t0, Tuple3<C, T> t1) {
//     return (1 - t) * t0 + t * t1;
// }

// template <template <class> class C, typename T>
// inline C<T> FMA(Float a, Tuple3<C, T> b, Tuple3<C, T> c) {
//     return {FMA(a, b.x, c.x), FMA(a, b.y, c.y), FMA(a, b.z, c.z)};
// }

// template <template <class> class C, typename T>
// inline C<T> FMA(Tuple3<C, T> a, Float b, Tuple3<C, T> c) {
//     return FMA(b, a, c);
// }

// template <template <class> class C, typename T>
// inline C<T> Min(Tuple3<C, T> t1, Tuple3<C, T> t2) {
//     using std::min;
//     return {min(t1.x, t2.x), min(t1.y, t2.y), min(t1.z, t2.z)};
// }

// template <template <class> class C, typename T>
// inline C<T> Max(Tuple3<C, T> t1, Tuple3<C, T> t2) {
//     using std::max;
//     return {max(t1.x, t2.x), max(t1.y, t2.y), max(t1.z, t2.z)};
// }

template <typename T>
class Vector3 : public Tuple3<Vector3, T> {
public:
    using Tuple3<Vector3, T>::x;
    using Tuple3<Vector3, T>::y;
    using Tuple3<Vector3, T>::z;
    Vector3(T x, T y, T z) : Tuple3<Vector3, T>(x, y, z) {}

    template <typename U>
    explicit Vector3(Vector3<U> v)
        : Tuple3<Vector3, T>(T(v.x), T(v.y), T(v.z)) {}
};

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;