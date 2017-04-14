#pragma once

#include <stddef.h>

#include "Geometry.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"


/*
 * Author: Ben Vesel
 * Date: 12/24/16
 * Classname: Triangle
 * Purpose: A triangle geometry class used for raytracing
 */
class Triangle : public Geometry {

	public :
		Triangle(Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<unsigned char> color, Material mat = MATERIAL_NONE);
		Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Vec3<unsigned char> color, Material mat = MATERIAL_NONE);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos);

	private : 
		Vec3<float> _vertexA;
		Vec3<float> _vertexB;
		Vec3<float> _vertexC;
		Vec3<float> _normal;

		typedef Geometry super;

};
