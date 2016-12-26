#pragma once
#include "Color.hpp"
#include "Material.hpp"

class RayHit {

	public :
		RayHit(float t, Material mat, Color col, std::shared_ptr<Vec3<float> > norm, std::shared_ptr<Vec3<float> > loc, std::shared_ptr<Vec3<float> > r);
		Material getMaterial();
		Color getColor();
		float getTime();
		std::shared_ptr<Vec3<float> > getNormal();
		std::shared_ptr<Vec3<float> > getHitLocation();
		std::shared_ptr<Vec3<float> > getRay();	

	private :
		float time;
		Material material;
		Color color;
		std::shared_ptr<Vec3<float> > normal;
		std::shared_ptr<Vec3<float> > hitLocation;
		std::shared_ptr<Vec3<float> > ray;
};
