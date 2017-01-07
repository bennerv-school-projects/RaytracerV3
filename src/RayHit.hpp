#pragma once
#include "Color.hpp"
#include "Material.hpp"

class RayHit {

	public :
		RayHit(float t, Material mat, Vec3<unsigned char> color, Vec3<float> norm, Vec3<float> loc, Vec3<float> r);
		Material GetMaterial();
		Color GetColor();
		float GetTime();
		Vec3<float> GetNormal();
		Vec3<float> GetHitLocation();
		Vec3<float> GetRay();	

	private :
		float _time;
		Material _material;
		Vec3<unsigned char> _color;
		Vec3<float> _normal;
		Vec3<float> _hitLocation;
		Vec3<float> _ray;
};
