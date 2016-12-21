#pragma once
#include <math.h>

template <class T>
class Vec3 {

	public :
		T x;
		T y;
		T z;

		Vec3(T a, T b, T c) {
			x = a;
			y = b;
			z = c;
		}

		static Vec3<T> * vec3(T x, T y, T z) {
			Vec3<T> * vector = new Vec3<T>(x, y, z);
			return vector;
		}

		static Vec3<T> * cross(Vec3 * a, Vec3 * b) {
			return Vec3::vec3(a->y * b->z - a->z * b->y, a->z * b->x - a->x * b->z, a->x * b->y - a->y * b->x);
		}

		static T dot(Vec3 * a, Vec3 * b) {
			return a->x * b->x + a->y * b->y + a->z * b->z;
		}

		static Vec3<T> * add(Vec3 * a, Vec3 * b) {
			return Vec3::vec3(a->x + b->x, a->y + b->y, a->z + b->z);
		}

		static Vec3<T> * sub(Vec3 * a, Vec3 * b) {
			return Vec3::vec3(a->x - b->x, a->y - b->y, a->z - b->z);
		}

		static Vec3<T> * normalize(Vec3 * a) {
			double temp = Vec3::magnitude(a);
			return Vec3::vec3(a->x / temp, a->y / temp, a->z / temp);
		}

		static double magnitude(Vec3 * a) {
			return sqrt( (double)(a->x * a->x + a->y * a->y + a->z * a->z));
		}

};

template <class T>
class Vec2 {

	public :
		T x;
		T y;

		Vec2(T a, T b) {
			x = a;
			y = b;
		}

		static Vec2<T> * vec2(T x, T y) {
			Vec2<T> * vector = new Vec2<T>(x, y);
			return vector;
		}

		static T dot(Vec2 * a, Vec2 * b) {
			return a->x * b->x + a->y * b->y;
		}

		static Vec2<T> * add(Vec2 * a, Vec2 * b) {
			return Vec2::vec2(a->x + b->x, a->y + b->y);
		}

		static Vec2<T> * sub(Vec2 * a, Vec2 * b) {
			return Vec2::vec2(a->x - b->x, a->y - b->y);
		}

		static Vec2<T> * normalize(Vec2 * a) {
			double temp = Vec2::magnitude(a);
			return Vec2::vec2(a->x / temp, a->y / temp);
		}

		static double magnitude(Vec2 * a) {
			return sqrt( (double)(a->x * a->x + a->y * a->y));
		}

};
