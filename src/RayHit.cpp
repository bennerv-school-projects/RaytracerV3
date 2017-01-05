#include "RayHit.hpp"

/*
 * Date: 12/28/16
 * Function Name: RayHit (constructor)
 * Arguments:
 *     float                    - time to hit
 *     Material                 - material of the intesected object
 *     Color                    - color of the intersected object
 *     Vec3<float> - the normal of the intersected object
 *     Vec3<float> - the hit location of the intersected object
 *     Vec3<float> - the original ray 
 * Purpose: Constructor
 * Return Value: void
*/
RayHit::RayHit(float t, Material mat, Color col, Vec3<float> norm, Vec3<float> loc, Vec3<float> r) : color(col.getColor()) {
	time = t;
	material = mat;
	normal = Vec3<float>::vec3(norm.x, norm.y, norm.z);
	hitLocation = Vec3<float>::vec3(loc.x, loc.y, loc.z);
	ray = Vec3<float>::vec3(r.x, r.y, r.z);
}

/*
 * Date: 12/28/16
 * Function Name: getNormal()
 * Arguments:
 *     void
 * Purpose: Returns the normal of the ray hit object
 * Return Value: Vec3<float>
 */
Vec3<float> RayHit::getNormal() {
	return normal;
}

/*
 * Date: 12/28/16
 * Function Name: getHitLocation()
 * Arguments:
 *     void
 * Purpose: Returns the location of the intersection
 * Return Value: Vec3<float>
 */
Vec3<float> RayHit::getHitLocation() {
	return hitLocation;
}

/*
 * Date: 12/28/16
 * Function Name: getRay()
 * Arguments:
 *     void
 * Purpose: Returns the ray that intersected with the object
 * Return Value: Vec3<float>
 */
Vec3<float> RayHit::getRay() {
	return ray;
}
