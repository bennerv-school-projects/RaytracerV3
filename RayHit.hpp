#pragma once
#include "Color.hpp"
#include "Material.hpp"

class RayHit {

	public :
		RayHit();
		~RayHit();			
	

	private :
		float time;
		Material material;
		Color color;
		Vec3<float> normal;
		Vec3<float> location;
		Vec3<float> ray;
};
