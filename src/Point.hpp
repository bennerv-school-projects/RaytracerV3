#pragma once

#include <stddef.h>

#include "Geometry.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 2/26/17
 * Classname: Point
 * Purpose: A point class used for raytracing
 */

class Point : public Geometry {

	public :
		Point(Vec3<float> pos);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos);
		Vec3<float> GetRandomPoint();


	private :
		Vec3<float> _position;
		typedef Geometry super;
};