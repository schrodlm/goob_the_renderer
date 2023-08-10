#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <iostream>

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

  inline Vec2<T> operator+(const Vec2<T> &V) const {
    return Vec2<T>{u + V.u, v + V.v};
  }
  inline Vec2<T> operator-(const Vec2<T> &V) const {
    return Vec2<T>{u - V.u, v + V.v};
  }
  inline Vec2<T> operator*(float f) const { return Vec2<T>{u * f, v * f}; }
  template <typename>
  friend std::ostream &operator<<(std::ostream &s, Vec2<T> &V);
};

template <typename T> struct Vec3 {};

#endif // !__GEOMETRY_H__
#define __GEOMETRY_H__
