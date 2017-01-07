#include "Triangle.hpp"

/* 
 * Date: 12/24/16
 * Function Name: Triangle (constructor)
 * Arguments:
 *     Vec3<float> - the first vertex
 *     Vec3<float> - the second vertex
 *     Vec3<float> - the third vertex
 *     Material    - the material of the triangle
 *     Colors      - the color of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Triangle::Triangle(Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<unsigned char> color, Material mat) {
	_vertexA = Vec3<float>::vec3(a.x, a.y, a.z);
	_vertexB = Vec3<float>::vec3(b.x, b.y, b.z);
	_vertexC = Vec3<float>::vec3(c.x, c.y, c.z);
	Vec3<float> temp = Vec3<float>::normalize(Vec3<float>::cross(Vec3<float>::sub(_vertexB, _vertexA), Vec3<float>::sub(_vertexC, _vertexA)));
	_normal = Vec3<float>::vec3(temp.x, temp.y, temp.z);
	_material = mat;
	_color = color;
}

/* 
 * Date: 12/24/16
 * Function Name: Triangle (constructor)
 * Arguments:
 *     float    - the first vertex x component
 *     float    - the first vertex y component
 *     float    - the first vertex z component
 *     float    - the second vertex x component
 *     float    - the second vertex y component
 *     float    - the second vertex z component
 *     float    - the third vertex x component
 *     float    - the third vertex y component
 *     float    - the third vertex z component
 *     Material - the material of the triangle
 *     Colors   - the color of the triangle
 * Purpose: Constructor 
 * Return Value: void
 */
Triangle::Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Vec3<unsigned char> color, Material mat) {
	_vertexA = Vec3<float>::vec3(ax, ay, az);
	_vertexB = Vec3<float>::vec3(bx, by, bz);
	_vertexC = Vec3<float>::vec3(cx, cy, cz);
	Vec3<float> temp = Vec3<float>::normalize(Vec3<float>::cross(Vec3<float>::sub(_vertexB, _vertexA), Vec3<float>::sub(_vertexC, _vertexA)));
	_normal = Vec3<float>::vec3(temp.x, temp.y, temp.z);
	_material = mat;
	_color = color;
}

/* 
 * Date: 12/24/16
 * Function Name: setMaterial
 * Arguments:
 *     Material - the type of material the triangle is made of
 * Purpose: Allows one to set the material of the triangle 
 * Return Value: void
 */
void Triangle::SetMaterial(Material mat) {
	_material = mat;
}

/* 
 * Date: 12/24/16
 * Function Name: intersect
 * Arguments:
 *     Vec3<float> - the ray
 *	   Vec3<float> - the starting position of the ray
 * Purpose: Gives the intersection between a triangle and the incoming ray 
 * Return Value: RayHit
 */
std::shared_ptr<RayHit> Triangle::Intersect(Vec3<float> ray, Vec3<float> startingPos) {
	float A = _vertexA.x - _vertexB.x;
	float B = _vertexA.y - _vertexB.y;
	float C = _vertexA.z - _vertexB.z;
	float D = _vertexA.x - _vertexC.x;
	float E = _vertexA.y - _vertexC.y;
	float F = _vertexA.z - _vertexC.z;

	float G = ray.x;
	float H = ray.y;
	float I = ray.z;
	
	float J = _vertexA.x - startingPos.x;
	float K = _vertexA.y - startingPos.y;
	float L = _vertexA.z - startingPos.z;

	float M = A * (E * I - H * F) + B * (G * F - D * I) + C * (D * H - E * G);
	float t = (-1 * (F * (A * K - J * B) + E * (J * C - A * L) + D * (B * L - K * C) ) ) / M;

	if( t < 0 ) {
		return nullptr;
	}
	float gamma = (I * (A * K - J * B) + H * (J * C - A * L) + G * (B * L - K * C)) / M;
	
	if( gamma < 0 || gamma > 1 ) {
		return nullptr;
	}
	float beta = (J * (E * I - H * F) + K * (G * F - D * I) + L * (D * H - E * G)) / M;
	
	if( beta < 0 || beta > 1 - gamma ) {
		return nullptr;
	}
	
	Vec3<float> hitLocation = Vec3<float>::add(Vec3<float>::vec3(t * ray.x, t * ray.y, t* ray.z), startingPos);
	std::shared_ptr<RayHit> rayHit(new RayHit(t, _material, _color, _normal, hitLocation, ray));
	 
	return rayHit;
}
