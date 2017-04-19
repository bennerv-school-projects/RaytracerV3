#include "Square.hpp"

/* 
 * Date: 4/17/17
 * Function Name: Square (constructor)
 * Arguments:
 * 	   Vec3<float> - one point on the square
 * 	   Vec3<float> - one point on the square
 * 	   Vec3<float> - one point on the square
 * 	   Vec3<float> - one point on the square
 *	   Vec3        - the color component
 *     Material    - the material of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Square::Square(Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<float> d, Vec3<unsigned char> color, Material mat) : super(SQUARE), _firstTriangle(a, b, c, color, mat), _secondTriangle(c, b, d, color, mat) {
	
	SetMaterial(mat);
	SetColor(color);
}


/* 
 * Date: 4/17/17
 * Function Name: intersect
 * Arguments:
 *     Vec3<float> - the ray
 *	   Vec3<float> - the starting position of the ray
 * Purpose: Gives the intersection between a triangle and the incoming ray 
 * Return Value: RayHit
 */
std::shared_ptr<RayHit> Square::Intersect(Vec3<float> ray, Vec3<float> startingPos) {
	
	std::shared_ptr<RayHit> rayHit1 = _firstTriangle.Intersect(ray, startingPos);
	std::shared_ptr<RayHit> rayHit2 = _secondTriangle.Intersect(ray, startingPos);
	
	if(rayHit2 == nullptr) {
		return rayHit1;
	}

	if(rayHit1 == nullptr) {
		return rayHit2;
	}

	float time1 = rayHit1->GetTime();
	float time2 = rayHit2->GetTime();

	if(time1 < 0.0005f) {
		return rayHit2;
	} else if(time2 < 0.0005f) {
		return rayHit1;
	} else {
        return time1 < time2 ? rayHit1 : rayHit2;
	}
}

