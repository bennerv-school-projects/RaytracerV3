#pragma once

#include <stddef.h>

#include "Color.hpp"
#include "Material.hpp"
#include "RayHit.hpp"
#include "Vector.hpp"


/*
 * Author: Ben Vesel
 * Date: 12/24/16
 * Classname: Triangle
 * Purpose: A triangle geometry class used for raytracing
 */
class Triangle {

	public :
		Triangle(std::shared_ptr<Vec3<float> > a, std::shared_ptr<Vec3<float> > b, std::shared_ptr<Vec3<float> > c, Material mat = NONE, Colors col = RED);
		Triangle(float ax, float ay, float az, float bx, float by, float bz, float cx, float cy, float cz, Material mat = NONE, Colors col = RED);
		std::shared_ptr<RayHit> intersect(std::shared_ptr<Vec3<float> > ray, std::shared_ptr<Vec3<float> > startingPos);
		void setMaterial(Material mat);

	private : 
		std::shared_ptr<Vec3<float> > vertexA;
		std::shared_ptr<Vec3<float> > vertexB;
		std::shared_ptr<Vec3<float> > vertexC;
		std::shared_ptr<Vec3<float> > normal;

		Material material;
		Color color;
};
