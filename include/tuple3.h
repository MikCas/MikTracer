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
        static const int nDimensions = 3;
        
        // Accessors
        T operator[](int i) const {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }
        
        T &operator[](int i) {
            if (i == 0) return x;
            if (i == 1) return y;
            return z;
        }
        
        // Arithmetic operations
        template <typename U>
        auto operator+(Child<U> c) const -> Child<decltype(T{} + U{})> {
            return {x + c.x, y + c.y, z + c.z};
        }

        template <typename U>
        Child<T> &operator+=(Child<U> c) {
            DCHECK(!c.HasNAN());
            x += c.x;
            y += c.y;
            z += c.z;
            return static_cast<Child<T> &>(*this);
        }

        template <typename U>
        auto operator-(Child<U> c) const -> Child<decltype(T{} - U{})> {
            DCHECK(!c.HasNAN());
            return {x - c.x, y - c.y, z - c.z};
        }

        template <typename U>
        Child<T> &operator-=(Child<U> c) {
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

       Child<T> abs() const {
           return {std::abs(x), std::abs(y), std::abs(z)};
       }

       // Comparison operators
       bool operator==(Child<T> c) const {
        return x == c.x && y == c.y && z == c.z;
       }

       bool operator!=(Child<T> c) const {
        return x != c.x || y != c.y || z != c.z; 
       }
    
       // String representation
       std::string ToString() const {return "(" + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z) + ")"; }

        T x{}, y{}, z{};
};