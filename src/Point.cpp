#include "Point.hpp"

/*
 * Date: 1/26/17
 * Function Name: Point (constructor)
 * Arguments: 
 *		Vec3<float> - the position of the point
 * Purpose: Constructor
 * Return Value: void
 */
Point::Point(Vec3<float> pos) : super(POINT) {
	_position = pos;
	Material mat = NONE;
	SetMaterial(mat);
	SetColor(Vec3<unsigned char>::vec3(255, 255, 255));
}

std::shared_ptr<RayHit> Point::Intersect(Vec3<float> ray, Vec3<float> startingPos) {
	return nullptr;
}