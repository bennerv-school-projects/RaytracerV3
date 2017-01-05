#include <cassert>
#include <memory>

#include "../src/Vector.hpp"

using namespace std;

int main(int argc, char * argv[]) {
	Vec3<float> temp0(1, 2, 3);
	Vec3<float> temp1(0, 0, 0);
	Vec3<float> temp2(1, 2, 3);

	Vec2<float> temp3(1, 2);
	Vec2<float> temp4(0, 0);
	Vec2<float> temp5(1, 2);
	
	float temp = Vec3<float>::dot(temp0, temp1);
	assert(temp == 0.f);

	temp = Vec3<float>::dot(temp0, temp2);
	assert(temp == 14.f);

	temp = Vec2<float>::dot(temp3, temp4);
	assert(temp == 0.f);

	temp = Vec2<float>::dot(temp3, temp5);
	assert(temp == 5.f);
}