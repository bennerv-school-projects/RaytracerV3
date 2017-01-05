#include "Sphere.hpp"

/* 
 * Date: 12/26/16
 * Function Name: Sphere (constructor)
 * Arguments:
 *     Vec3<float> - the center of the sphere
 *     float                    - the radius of the sphere
 *     Material                 - the material of the triangle
 *     Colors                   - the color of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Sphere::Sphere(Vec3<float> a, float r, Material mat, Colors col) : color(col) {
	center = Vec3<float>::vec3(a.x, a.y, a.z); 
	radius = r;
	material = mat;
}

/* 
 * Date: 12/26/16
 * Function Name: Sphere (constructor)
 * Arguments:
 *     float    - x component of sphere's center coord
 *     float    - y component of sphere's center coord
 *     float    - z component of sphere's center coord
 *     float    - the radius of the sphere
 *     Material - the material of the triangle
 *     Colors   - the color of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Sphere::Sphere(float ax, float ay, float az, float r, Material mat, Colors col) : color(col) {
	center = Vec3<float>::vec3(ax, ay, az); 
	radius = r;
	material = mat;
	color = Color(col);
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
std::shared_ptr<RayHit> Sphere::intersect(Vec3<float> ray, Vec3<float> startingPos) {
	// d = ray, e = starting pos, c = center
	return nullptr;
}

void Sphere::setMaterial(Material mat) {
	material = mat;
}