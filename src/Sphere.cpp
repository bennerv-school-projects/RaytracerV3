#include "Sphere.hpp"

/* 
 * Date: 12/26/16
 * Function Name: Sphere (constructor)
 * Arguments:
 *     Vec3<float> - the center of the sphere
 *     float                    - the radius of the sphere
 *	   Vec3                     - the color component
 *     Material                 - the material of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Sphere::Sphere(Vec3<float> a, float r, Vec3<unsigned char> color, Material mat) {
	_center = Vec3<float>::vec3(a.x, a.y, a.z); 
	_radius = r;
	_material = mat;
	_color = color;
}

/* 
 * Date: 12/26/16
 * Function Name: Sphere (constructor)
 * Arguments:
 *     float    - x component of sphere's center coord
 *     float    - y component of sphere's center coord
 *     float    - z component of sphere's center coord
 *     float    - the radius of the sphere
 *	   Vec3     - the color component
 *     Material - the material of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Sphere::Sphere(float ax, float ay, float az, float r, Vec3<unsigned char> color, Material mat) {
	_center = Vec3<float>::vec3(ax, ay, az); 
	_radius = r;
	_material = mat;
	_color = color;
}

/* 
 * Date: 12/26/16
 * Function Name: intersect (constructor)
 * Arguments:
 *     Vec3<float> - the ray potentially intersecting
 *     Vec3<float> - the starting location of the ray
 * Purpose: Intersection code for a sphere 
 * Return Value: RayHit
 */
std::shared_ptr<RayHit> Sphere::Intersect(Vec3<float> ray, Vec3<float> startingPos) {
	// d = ray, e = starting pos, c = center
	return nullptr;
}

void Sphere::SetMaterial(Material mat) {
	_material = mat;
}