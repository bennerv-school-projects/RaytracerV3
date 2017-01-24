#pragma once

#include "RayHit.hpp"
#include "Vector.hpp"

class Geometry {
	public :

		enum Shape {
			TRIANGLE,
			SPHERE, 
			POINT
		};

		/* 
		 * Date: 1/18/16
		 * Function Name: Geometry (constructor)
		 * Arguments: 
		 *     Shape - the shape of the geometry
		 * Purpose: Constructor
		 * Return Value: void
		 */
		Geometry(Shape shape) {
			_shape = shape;
		}

		/* 
		 * Date: 1/18/16
		 * Function Name: GetColor
		 * Arguments: 
		 *     void
		 * Purpose: Gets the color
		 * Return Value: Vec3<unsigned char>
		 */
		Vec3<unsigned char> GetColor() {
			return _color;
		}

		/* 
		 * Date: 1/18/16
		 * Function Name: GetMaterial
		 * Arguments: 
		 *     void
		 * Purpose: Gets the material of the geometry
		 * Return Value: Material
		 */
		Material GetMaterial() {
			return _material;
		}

		/* 
		 * Date: 1/18/16
		 * Function Name: GetShape
		 * Arguments: 
		 *     void
		 * Purpose: Returns the shape of the material
		 * Return Value: Shape
		 */
		Shape GetShape() {
			return _shape;
		}

		/* 
		 * Date: 1/18/16
		 * Function Name: SetColor
		 * Arguments: 
		 *     Vec3<unsigned char> - the color of the geometry
		 * Purpose: Sets the color of the geometry
		 * Return Value: void
		 */
		void SetColor(Vec3<unsigned char> color) {
			_color = color;
		}

		/* 
		 * Date: 1/18/16
		 * Function Name: SetMaterial
		 * Arguments: 
		 *     Material - the material of the geometry
		 * Purpose: Sets the material of the geometry
		 * Return Value: void
		 */
		void SetMaterial(Material mat) {
			_material = mat;
		}
		
		/* 
		 * Date: 1/18/16
		 * Function Name: Intersect
		 * Arguments: 
		 *     Vec3<float> - the ray
		 *     Vec3<float> - the starting position of the ray
		 * Purpose: Returns a RayHit object on the intersection of geometry
		 * Return Value: shared_ptr<RayHit>
		 */
		virtual std::shared_ptr<RayHit> Intersect(Vec3<float> ray, Vec3<float> startingPos) {
			return nullptr;
		}

	private :
		Vec3<unsigned char> _color;
		Material _material;
		Shape _shape;

};