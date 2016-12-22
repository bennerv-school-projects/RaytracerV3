#pragma once
#include "Vector.hpp"

class Triangle {

	public :
		Triangle(Vec3<float> * a, Vec3<float> * b, Vec3<float> * c);
		Triangle(Vec3<float> * a, Vec3<float> * b, Vec3<float> * c, Material mat, Color c);
		Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz);
		Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Material mat, Color c);
		~Triangle();
		float intersect(Vec3<float> * ray, Vec3<float> * startingPos);
		void setMaterial(Material mat);
		void setColor(Color c);
	private: 
		Vec3<float> * vertexA;
		Vec3<float> * vertexB;
		Vec3<float> * vertexC;

		Material material = Material::NONE;
		Color color;
};
