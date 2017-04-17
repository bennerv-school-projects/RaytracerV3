#pragma once
#include "Vector.hpp"
#include "Material.hpp"

class RayHit {

	public :
		RayHit(float t, Material mat, Vec3<unsigned char> color, Vec3<float> norm, Vec3<float> loc, Vec3<float> r);
		RayHit(float t, Material mat, Vec3<unsigned char> color, Vec3<float> norm, Vec3<float> secondNorm, Vec3<float> loc, Vec3<float> r);

		Vec3<unsigned char> GetColor();
		Vec3<float> GetHitLocation();
		Material GetMaterial();
		Vec3<float> GetNormal();
		Vec3<float> GetSecondaryNormal();
		Vec3<float> GetRay();	
		float GetTime();

	private :
		float _time;
		Material _material;
		Vec3<unsigned char> _color;
		Vec3<float> _normal;
		Vec3<float> _secondNorm;
		Vec3<float> _hitLocation;
		Vec3<float> _ray;
};
