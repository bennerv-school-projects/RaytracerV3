#pragma once

#include "RayHit.hpp"
#include "Vector.hpp"

class Geometry {
	public :

		enum Shape {
			TRIANGLE,
			SPHERE
		};

		Vec3<unsigned char> GetColor() {
			return _color;
		}

		Material GetMaterial() {
			return _material;
		}

		Shape GetShape() {
			return _shape;
		}

		void SetColor(Vec3<unsigned char> color) {
			_color = color;
		}

		void SetMaterial(Material mat) {
			_material = mat;
		}
		
		virtual std::shared_ptr<RayHit> Intersect(Vec3<float>, Vec3<float> startingPos) {
			return nullptr;
		}

	private :
		Vec3<unsigned char> _color;
		Material _material;
		Shape _shape;

};