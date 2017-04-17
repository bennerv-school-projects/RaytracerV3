#pragma once

#include <stddef.h>

#include "Geometry.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 4/17/16
 * Classname: Square
 * Purpose: A square geometry class used for raytracing
 */
class Square : public Geometry {

	public :
		Square(Vec3<float> a, Vec3<float> b, Vec3<float> c, Vec3<float> d, Vec3<unsigned char> color, Material mat = MATERIAL_NONE);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPosition);

	private:
		Triangle _firstTriangle;
		Triangle _secondTriangle;

		typedef Geometry super;
};
