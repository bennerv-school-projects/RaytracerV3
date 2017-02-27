#include "Sphere.hpp"

/* 
 * Date: 12/26/16
 * Function Name: Sphere (constructor)
 * Arguments:
 *     Vec3<float> - the center of the sphere
 *     float       - the radius of the sphere
 *	   Vec3        - the color component
 *     Material    - the material of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Sphere::Sphere(Vec3<float> a, float r, Vec3<unsigned char> color, Material mat) : super(SPHERE) {
	_center = Vec3<float>::vec3(a.x, a.y, a.z); 
	_radius = r;
	SetMaterial(mat);
	SetColor(color);
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
Sphere::Sphere(float ax, float ay, float az, float r, Vec3<unsigned char> color, Material mat) : super(SPHERE) {
	_center = Vec3<float>::vec3(ax, ay, az); 
	_radius = r;
	SetMaterial(mat);
	SetColor(color);
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
	float discriminate = pow(ray * (startingPos - _center), 2) - ((ray * ray) * (((startingPos - _center) * (startingPos - _center)) - (_radius * _radius)));

	if (discriminate < 0) {
		return nullptr;
	}
	
	discriminate = sqrtf(discriminate);
	Vec3<float> zeroVector(0, 0, 0);

	float time0 = (((zeroVector - ray) * (startingPos - _center)) + discriminate) / (ray * ray);
	float time1 = (((zeroVector - ray) * (startingPos - _center)) - discriminate) / (ray * ray);

	if (time0 < 0 && time1 < 1) {
		return nullptr;
	}
	float trueTime = time0;

	if (time0 < 0 || (time1 > 0 && time1 < time0)) {
		trueTime = time1;
	}

	Vec3<float> hitLocation = (ray * trueTime) + startingPos;
	Vec3<float> normal = Vec3<float>::Normalize(hitLocation - _center);

	std::shared_ptr<RayHit> rayHit(new RayHit(trueTime, GetMaterial(), GetColor(), normal, hitLocation, ray));
	return rayHit;
}