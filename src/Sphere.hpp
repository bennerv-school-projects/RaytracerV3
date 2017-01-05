#pragma once

#include <stddef.h>

#include "Color.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"

class Sphere {
	
	public: 
		Sphere(Vec3<float> a, float r, Material mat = NONE, Colors col = RED);
		Sphere(float ax, float ay, float az, float r, Material mat = NONE, Colors col = RED);
		std::shared_ptr<RayHit> intersect(Vec3<float> ray, Vec3<float> startingPos);
		void setMaterial(Material mat);

	private :
		Vec3<float> center;
		Color color;
		Material material;
		float radius;
};