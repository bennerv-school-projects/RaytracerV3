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
	Material mat = MATERIAL_NONE;
	SetMaterial(mat);
	SetColor(Vec3<unsigned char>::vec3(255, 255, 255));
}

/* 
 * Date: 2/27/17
 * Function Name: Intersect
 * Arguments:
 *		Vec3<float> - the ray being cast
 *		Vec3<float> - the starting position of the ray
 * Return Value: void
 */
std::shared_ptr<RayHit> Point::Intersect(Vec3<float> ray, Vec3<float> startingPos) {
	return nullptr;
}

/*
 * Date: 2/27/17
 * Function Name: GetRandomPoint
 * Arguments:
 *		void
 * Return Value: Vec3<float>
 */
Vec3<float> Point::GetRandomPoint() {
	return _position;
}
