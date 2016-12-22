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
		Color(Colors c) {
			switch(c) {
				case RED :
					color = new Vec3<int>(255, 0, 0);
					break;
				case BLUE :
					color = new Vec3<int>(0, 0, 255);
					break;
				case GREEN :
					color = new Vec3<int>(0, 255, 0);
					break;
				case WHITE :
					color = new Vec3<int>(255, 255, 255);
					break;
				case BLACK :
					color = new Vec3<int>(0, 0, 0);
					break;
				default :
					color = new Vec3<int>(255, 255, 255);
			}
		}

		~Color() {
			delete color;
		}

		
	private :
		Vec3<int> * color;
};
