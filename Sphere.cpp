#include "Sphere.hpp"

Sphere::Sphere(Vec3<float> * a, float r, Material mat, Colors col) : center(0, 0, 0), color(col) {
	center = Vec3<float>(a->x, a->y, a->z);
	radius = r;
	material = mat;
}

Sphere::Sphere(float ax, float ay, float az, float r, Material mat, Colors col) : center(0, 0, 0), color(col) {
	center = Vec3<float>(ax, ay, az);
	radius = r;
	material = mat;
	color = Color(col);
}

RayHit * Sphere::intersect(Vec3<float> * ray, Vec3<float> * startingPos) {
	RayHit * rayHit;
	// d = ray, e = starting pos, c = center
	return NULL;
}

void Sphere::setMaterial(Material mat) {
	material = mat;
}