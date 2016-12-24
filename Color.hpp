#pragma once
#include "Vector.hpp"

enum Colors { RED, BLUE, WHITE, BLACK, GREEN };

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Color
 * Purpose: A class that contains colors for objects
 */
class Color {
	public: 

		/* 
		 * Date: 12/24/16
		 * Function Name: Color (constructor)
		 * Arguments:
		 *     Colors - a color enumeration
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Color(Colors c) : color(0, 0, 0) {
			switch(c) {
				case RED :
					color = Vec3<unsigned char>(255, 0, 0);
					break;
				case BLUE :
					color = Vec3<unsigned char>(0, 0, 255);
					break;
				case GREEN :
					color = Vec3<unsigned char>(0, 255, 0);
					break;
				case WHITE :
					color = Vec3<unsigned char>(255, 255, 255);
					break;
				case BLACK :
					color = Vec3<unsigned char>(0, 0, 0);
					break;
				default :
					color = Vec3<unsigned char>(255, 255, 255);
			}
		}

		/* 
		 * Date: 12/24/16
		 * Function Name: Color (constructor)
		 * Arguments:
		 *     Vec3<unsigned char> * - the color to use
		 * Purpose: Constructor 
		 * Return Value: void
		 */
		Color(Vec3<unsigned char> col) : color(0, 0, 0) {
			color = Vec3<unsigned char>(col.x, col.y, col.z);
		}

		/* 
		 * Date: 12/24/16
		 * Function Name: getColor
		 * Arguments:
		 *     void
		 * Purpose: Constructor 
		 * Return Value: Vec3<unsigned char>
		 */
		Vec3<unsigned char> getColor() {
			return color;
		}

	private :
		Vec3<unsigned char> color;
};
