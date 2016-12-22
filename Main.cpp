#include <stdio.h>
#include "Vector.hpp"


int main(int argc, char * argv[]) {
	Vec3<float> * vector = Vec3<float>::vec3(1, 1, 1);
	Vec3<float> * vector2 = Vec3<float>::vec3(2, 5, 23);
	
	Vec3<float> * result = Vec3<float>::cross(vector, vector2);
	printf("Cross product %f %f %f\n", result->x, result->y, result->z);
	delete result;

	result = Vec3<float>::add(vector, vector2);
	printf("Add product %f %f %f\n", result->x, result->y, result->z);
	delete result;	

	result = Vec3<float>::sub(vector, vector2);
	printf("Sub product %f %f %f\n", result->x, result->y, result->z);
	delete result;

	result = Vec3<float>::normalize(vector);
	printf("Normalize %f %f %f\n", result->x, result->y, result->z);
	delete result;

	float temp = (float)Vec3<float>::magnitude(vector);
	printf("Magnitude = %f\n", temp);

	temp = Vec3<float>::dot(vector, vector2);
	printf("Dot product = %f\n", temp);

	delete vector;
	delete vector2;
}