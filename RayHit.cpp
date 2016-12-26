#include "RayHit.hpp"

RayHit::RayHit(float t, Material mat, Color col, std::shared_ptr<Vec3<float> > norm, std::shared_ptr<Vec3<float> > loc, std::shared_ptr<Vec3<float> > r) : color(col.getColor()) {
	time = t;
	material = mat;
	normal = Vec3<float>::vec3(norm->x, norm->y, norm->z);
	hitLocation = Vec3<float>::vec3(loc->x, loc->y, loc->z);
	ray = Vec3<float>::vec3(r->x, r->y, r->z);
}

std::shared_ptr<Vec3<float> > RayHit::getNormal() {
	return normal;
}

std::shared_ptr<Vec3<float> > RayHit::getHitLocation() {
	return hitLocation;
}

std::shared_ptr<Vec3<float> > RayHit::getRay() {
	return ray;
}
