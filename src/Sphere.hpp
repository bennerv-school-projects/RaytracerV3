#pragma once

#include <stddef.h>

#include "Geometry.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"

class Sphere : public Geometry {
	
	public: 
		Sphere(Vec3<float> a, float r, Vec3<unsigned char> color, Material mat = NONE);
		Sphere(float ax, float ay, float az, float r, Vec3<unsigned char> color, Material mat = NONE);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos);

	private :
		Vec3<float> _center;
		float _radius;

		typedef Geometry super;
};