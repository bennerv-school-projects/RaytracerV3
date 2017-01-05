#pragma once
#include "Color.hpp"
#include "Material.hpp"

class RayHit {

	public :
		RayHit(float t, Material mat, Color col, Vec3<float> norm, Vec3<float> loc, Vec3<float> r);
		Material getMaterial();
		Color getColor();
		float getTime();
		Vec3<float> getNormal();
		Vec3<float> getHitLocation();
		Vec3<float> getRay();	

	private :
		float time;
		Material material;
		Color color;
		Vec3<float> normal;
		Vec3<float> hitLocation;
		Vec3<float> ray;
};
