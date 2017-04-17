#include "RayHit.hpp"

/*
 * Date: 12/28/16
 * Function Name: RayHit (constructor)
 * Arguments:
 *     float       - time to hit
 *     Material    - material of the intesected object
 *     Color       - color of the intersected object
 *     Vec3<float> - the normal of the intersected object
 *     Vec3<float> - the hit location of the intersected object
 *     Vec3<float> - the original ray 
 * Purpose: Constructor
 * Return Value: void
*/
RayHit::RayHit(float t, Material mat, Vec3<unsigned char> color, Vec3<float> norm, Vec3<float> loc, Vec3<float> r) : _secondNorm(0, 0, 0){
	_time = t;
	_material = mat;
	_normal = Vec3<float>::vec3(norm.x, norm.y, norm.z);
	_hitLocation = Vec3<float>::vec3(loc.x, loc.y, loc.z);
	_ray = Vec3<float>::vec3(r.x, r.y, r.z);
	_color = color;
}

/*
* Date: 12/28/16
* Function Name: RayHit (constructor)
* Arguments:
*     float       - time to hit
*     Material    - material of the intesected object
*     Color       - color of the intersected object
*     Vec3<float> - the normal of the intersected object
*     Vec3<float> - the secondary normal of the intersected object.  - of normal
*     Vec3<float> - the hit location of the intersected object
*     Vec3<float> - the original ray
* Purpose: Constructor
* Return Value: void
*/
RayHit::RayHit(float t, Material mat, Vec3<unsigned char> color, Vec3<float> norm, Vec3<float> secondNorm, Vec3<float> loc, Vec3<float> r) {
	_time = t;
	_material = mat;
	_normal = Vec3<float>::vec3(norm.x, norm.y, norm.z);
	_secondNorm = Vec3<float>::vec3(secondNorm.x, secondNorm.y, secondNorm.z);
	_hitLocation = Vec3<float>::vec3(loc.x, loc.y, loc.z);
	_ray = Vec3<float>::vec3(r.x, r.y, r.z);
	_color = color;
}

/*
* Date: 1/16/17
* Function Name: GetColor()
* Arguments:
*     void
* Purpose: Returns the color of the hit object
* Return Value: Vec3<unsigned char>
*/
Vec3<unsigned char> RayHit::GetColor() {
	return _color;
}

/*
* Date: 12/28/16
* Function Name: GetHitLocation()
* Arguments:
*     void
* Purpose: Returns the location of the intersection
* Return Value: Vec3<float>
*/
Vec3<float> RayHit::GetHitLocation() {
	return _hitLocation;
}

/*
* Date: 1/16/17
* Function Name: GetMaterial()
* Arguments:
*     void
* Purpose: Returns the material of the object hit
* Return Value: Material
*/
Material RayHit::GetMaterial() {
	return _material;
}

/*
 * Date: 12/28/16
 * Function Name: GetNormal()
 * Arguments:
 *     void
 * Purpose: Returns the normal of the ray hit object
 * Return Value: Vec3<float>
 */
Vec3<float> RayHit::GetNormal() {
	return _normal;
}

/*
* Date: 4/6/17
* Function Name: GetSecondaryNormal()
* Arguments:
*     void
* Purpose: Returns the secondary normal of the ray hit object
* Return Value: Vec3<float>
*/
Vec3<float> RayHit::GetSecondaryNormal() {
	return _secondNorm;
}

/*
 * Date: 12/28/16
 * Function Name: GetRay()
 * Arguments:
 *     void
 * Purpose: Returns the ray that intersected with the object
 * Return Value: Vec3<float>
 */
Vec3<float> RayHit::GetRay() {
	return _ray;
}

/*
* Date: 1/16/17
* Function Name: GetTime()
* Arguments:
*     void
* Purpose: Returns the time of the object that it hit
* Return Value: float
*/
float RayHit::GetTime() {
	return _time;
}