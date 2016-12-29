#include <stdio.h>

#include "stb_image_write.h"

#include "Vector.hpp"
#define STB_IMAGE_WRITE_IMPLEMENTATION

using namespace std;

int main(int argc, char * argv[]) {
	shared_ptr<Vec3<float> > vector = Vec3<float>::vec3(1, 1, 1);
	shared_ptr<Vec3<float> > vector2 = Vec3<float>::vec3(2, 5, 23);
	shared_ptr<Vec3<float> > vector3 = Vec3<float>::vec3(1, 1, 1);

	if (*vector == *vector3) {
		int temp = 1000;
		temp += 100;
	}
	
	shared_ptr<Vec3<float> > result = Vec3<float>::cross(vector, vector2);
	printf("Cross product %f %f %f\n", result->x, result->y, result->z);

	result = Vec3<float>::add(vector, vector2);
	printf("Add product %f %f %f\n", result->x, result->y, result->z);	

	result = Vec3<float>::sub(vector, vector2);
	printf("Sub product %f %f %f\n", result->x, result->y, result->z);

	result = Vec3<float>::normalize(vector);
	printf("Normalize %f %f %f\n", result->x, result->y, result->z);

	float temp = (float)Vec3<float>::magnitude(vector);
	printf("Magnitude = %f\n", temp);

	temp = Vec3<float>::dot(vector, vector2);
	printf("Dot product = %f\n", temp);
}
