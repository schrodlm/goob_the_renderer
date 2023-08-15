#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>
#include <iostream>
#include <ostream>

template <typename T> struct Vec2 {
  union {
    struct {
      T u, v;
    };
    struct {
      T x, y;
    };
    T raw[2];
  };

  Vec2() : u(0), v(0) {}
  Vec2(T _u, T _v) : u(_u), v(_v) {}

  //conversion constructor
  template <typename U>
    Vec2(const Vec2<U>& other) : x(static_cast<T>(other.x)), y(static_cast<T>(other.y)) {}

  inline Vec2<T> operator+(const Vec2<T> &V) const {
    return Vec2<T>{u + V.u, v + V.v};
  }
  inline Vec2<T> operator-(const Vec2<T> &V) const {
    return Vec2<T>{u - V.u, v + V.v};
  }
  inline Vec2<T> operator*(const float f) const {
    return Vec2<T>{u * f, v * f};
  }

  template <typename>
  friend std::ostream &operator<<(std::ostream &s, Vec2<T> &V);
};

template <typename T> struct Vec3 {
  union {
    struct {
      T x, y, z;
    };
    struct {
      T vert, uv, norm;
    };
    T raw[3];
  };

  Vec3() : x(0), y(0), z(0) {}
  Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

  // cross product
  inline Vec3<T> operator^(const Vec3<T> &V) const {
    return Vec3<T>{y * V.z - z * V.y, z * V.x - x * V.z, x * V.y - y * V.x};
  }
  inline Vec3<T> operator+(const Vec3<T> &V) const {
    return Vec3<T>{x + V.x, y + V.y, z + V.z};
  }
  inline Vec3<T> operator-(const Vec3<T> &V) const {
    return Vec3<T>{x - V.x, y - V.y, z - V.z};
  }
  inline Vec3<T> operator*(const float f) const {
    return Vec3<T>{x * f, y * f, z * f};
  }
  inline T operator*(const Vec3<T> &V) const {
    return x * V.x + y * V.y + z * V.z;
  }

  float length() { return std::sqrt(x * x + y * y + z * z); }

  Vec3<T> &normalize(T l = 1) {
    *this = (*this) * (l / length());
    return *this;
  }

  template <typename>
  friend std::ostream &operator<<(std::ostream &out, const Vec3<T> &V);
};

typedef Vec2<float> Vec2f;
typedef Vec2<int> Vec2i;
typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vec2<T> &V) {
  out << "(" << V.x << "," << V.y << ")" << std::endl;
  return out;
}

template <typename T>
std::ostream &operator<<(std::ostream &out, const Vec3<T> &V) {
  out << "(" << V.x << "," << V.y << "," << V.z << ")" << std::endl;
  return out;
}

#endif // !__GEOMETRY_H__
