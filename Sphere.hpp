#pragma once

#include <stddef.h>

#include "Color.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"

class Sphere {
	
	public: 
		Sphere(std::shared_ptr<Vec3<float> > a, float r, Material mat = NONE, Colors col = RED);
		Sphere(float ax, float ay, float az, float r, Material mat = NONE, Colors col = RED);
		std::shared_ptr<RayHit> intersect(std::shared_ptr<Vec3<float> > ray, std::shared_ptr<Vec3<float> > startingPos);
		void setMaterial(Material mat);

	private :
		std::shared_ptr<Vec3<float> > center;
		Color color;
		Material material;
		float radius;
};