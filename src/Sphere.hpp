#pragma once

#include <stddef.h>

#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"

class Sphere : public Geometry {
	
	public: 
		Sphere(Vec3<float> a, float r, Vec3<unsigned char> color, Material mat = NONE);
		Sphere(float ax, float ay, float az, float r, Vec3<unsigned char> color, Material mat = NONE);
		std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos);
		void SetMaterial(Material mat);

	private :
		Vec3<float> _center;
		Vec3<unsigned char> _color;
		Material _material;
		float _radius;
};