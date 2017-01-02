#include <stdio.h>
#include <iostream>

#if defined(__linux) || defined(__APPLE__)
	#include <unistd.h>
	#define TERM_NRM  "\x1B[0m"
	#define TERM_BLK  "\x1B[30m"
	#define TERM_RED  "\x1B[31m"
	#define TERM_GRN  "\x1B[32m"
	#define TERM_YEL  "\x1B[33m"
	#define TERM_BLU  "\x1B[34m"
	#define TERM_MAG  "\x1B[35m"
	#define TERM_CYN  "\x1B[36m"
	#define TERM_WHT  "\x1B[37m"
#endif

#include "stb_image_write.h"
#include "Vector.hpp"

using namespace std;

void setPixelColor(Vec3<unsigned char> color, Vec2<int> coordinate, unsigned char * array, int width) {

	int pos = (coordinate.y * 3 * width) + (coordinate.x * 3);

	array[pos] = color.x;
	array[++pos] = color.y;
	array[++pos] = color.z;
}

int main(int argc, char * argv[]) {

	int width = 512;
	int height = 512;

	unsigned char * imageArray = (unsigned char *) malloc(3 * width * height * sizeof(unsigned char));
	if( !imageArray ) {
		
#if defined(__linux) || defined(__APPLE__)
		if( isatty(fileno(stdout)) ) {
			cout << TERM_RED "Failed to allocate memory for the image array.  Exiting." TERM_NRM << endl;
		} else {
			cout << "Failed to allocate memory for the image array.  Exiting" << endl;
		}
#else 
		cout << "Failed to allocate memory for the image array.  Exiting" << endl;
#endif
		exit(1);
	}

}
