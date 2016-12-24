#include "RayHit.hpp"

RayHit::RayHit(float t, Material mat, Color col, Vec3<float> * norm, Vec3<float> * loc, Vec3<float> * r) : color(col.getColor()) {
	time = t;
	material = mat;
	normal = new Vec3<float>(norm->x, norm->y, norm->z);
	hitLocation = new Vec3<float>(loc->x, loc->y, loc->z);
	ray = new Vec3<float>(r->x, r->y, r->z);
}

RayHit::~RayHit() {
	delete normal;
	delete hitLocation;
	delete ray;
}

Vec3<float> * RayHit::getNormal() {
	return normal;
}

Vec3<float> * RayHit::getHitLocation() {
	return hitLocation;
}

Vec3<float> * RayHit::getRay() {
	return ray;
}
