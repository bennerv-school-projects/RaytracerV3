#pragma once
#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 12/20/16
 * Classname: Color
 * Purpose: A class that contains colors for objects
 */
class Color {
	public: 
		Vec3<int> RED    = Vec3<int>(255, 0,   0);
		Vec3<int> BLUE   = Vec3<int>(0,   0,   255);
		Vec3<int> GREEN  = Vec3<int>(0,   255, 0);
		Vec3<int> WHITE  = Vec3<int>(255, 255, 255); 
		Vec3<int> BLACK  = Vec3<int>(0,   0,   0);

};