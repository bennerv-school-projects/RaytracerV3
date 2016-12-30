#include <cassert>
#include <memory>

#include "../src/Vector.hpp"

using namespace std;

int main(int argc, char * argv[]) {
	shared_ptr<Vec3<float> > temp0(new Vec3<float>(1, 2, 3));
	shared_ptr<Vec3<float> > temp1(new Vec3<float>(0, 0, 0));
	shared_ptr<Vec3<float> > temp2(new Vec3<float>(1, 2, 3));

	shared_ptr<Vec2<float> > temp3(new Vec2<float>(1, 2));
	shared_ptr<Vec2<float> > temp4(new Vec2<float>(0, 0));
	shared_ptr<Vec2<float> > temp5(new Vec2<float>(1, 2));
	
	float temp = Vec3<float>::dot(temp0, temp1);
	assert(temp == 0.f);

	temp = Vec3<float>::dot(temp0, temp2);
	assert(temp == 14.f);

	temp = Vec2<float>::dot(temp3, temp4);
	assert(temp == 0.f);

	temp = Vec2<float>::dot(temp3, temp5);
	assert(temp == 5.f);
}