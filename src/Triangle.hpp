#pragma once

#include <stddef.h>

#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"


/*
 * Author: Ben Vesel
 * Date: 12/24/16
 * Classname: Triangle
 * Purpose: A triangle geometry class used for raytracing
 */
class Triangle {

	public :
		Triangle(Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<unsigned char> color, Material mat = NONE);
		Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Vec3<unsigned char> color, Material mat = NONE);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos);
		void SetMaterial(Material mat);

	private : 
		Vec3<float> _vertexA;
		Vec3<float> _vertexB;
		Vec3<float> _vertexC;
		Vec3<float> _normal;

		Material _material;
		Vec3<unsigned char> _color;
};
