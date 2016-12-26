#include "Sphere.hpp"

Sphere::Sphere(std::shared_ptr<Vec3<float> > a, float r, Material mat, Colors col) : color(col) {
	center = Vec3<float>::vec3(a->x, a->y, a->z); 
	radius = r;
	material = mat;
}

Sphere::Sphere(float ax, float ay, float az, float r, Material mat, Colors col) : color(col) {
	center = Vec3<float>::vec3(ax, ay, az); 
	radius = r;
	material = mat;
	color = Color(col);
}

std::shared_ptr<RayHit> Sphere::intersect(std::shared_ptr<Vec3<float> > ray, std::shared_ptr<Vec3<float> > startingPos) {
	// d = ray, e = starting pos, c = center
	return nullptr;
}

void Sphere::setMaterial(Material mat) {
	material = mat;
}