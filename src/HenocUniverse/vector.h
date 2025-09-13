/*
 * TT102 HENOC 
 * 2008 @ ESCOM-IPN
 *
 */
#ifndef _HENOC_VECTOR_H
#define _HENOC_VECTOR_H

#include <ode/ode.h>
#include <cmath>

namespace HenocUniverse{
	const dReal pi = 3.1415926535897932384626433832795;
	bool is_nan(dReal f);
	dReal round(dReal f);

	struct vec2;
	dReal dot(const vec2 &a, const vec2 &b);
	dReal cross(vec2 a, vec2 b);

	struct vec2{
		vec2() {}
		vec2(dReal x, dReal y) : x(x), y(y) {}
		vec2(const dReal *f) : x(f[0]), y(f[1]) {}
		dReal length() const;
		vec2 rotate(dReal d) const;
		dReal squared_length() const { return dot(*this, *this); }
		void normalize() { *this /= length(); }
		vec2 rotate(vec2 v) const { return vec2(x * v.x + y * v.y, v.x * y - x * v.y); }
		vec2 hat() const { return *this / length(); }
		vec2 perp() const { return vec2(-y, x); }
		vec2 flip() const { return vec2(y, x); }
		void snap(dReal epsilon);
		vec2 operator-() const { return vec2(-x, -y); }
		vec2 &operator-=(vec2 v) { x -= v.x; y -= v.y; return *this; }
		vec2 &operator+=(vec2 v) { x += v.x; y += v.y; return *this; }
		vec2 &operator/=(dReal f) { x /= f; y /= f; return *this; }
		vec2 operator+(vec2 v) const { return vec2(x + v.x, y + v.y); }
		vec2 operator-(vec2 v) const { return vec2(x - v.x, y - v.y); }
		vec2 operator*(dReal f) const { return vec2(x * f, y * f); }
		vec2 operator/(dReal f) const { return vec2(x / f, y / f); }
		dReal x, y;
	};

	inline dReal dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }
	inline dReal cross(vec2 a, vec2 b) { return a.x * b.y - a.y * b.x; }
}

#endif
