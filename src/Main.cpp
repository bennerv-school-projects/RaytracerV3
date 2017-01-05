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

#include "INIReader.h"
#include "stb_image_write.h"
#include "Vector.hpp"

using namespace std;

void setPixelColor(Vec3<unsigned char> color, Vec2<int> coordinate, unsigned char * array, int width) {

	int pos = (coordinate.y * 3 * width) + (coordinate.x * 3);

	array[pos] = color.x;
	array[++pos] = color.y;
	array[++pos] = color.z;
}

void getPixelColor(Vec3<unsigned char> &color, Vec2<int> coordinate, unsigned char * array, int width) {
	int pos = (coordinate.y * 3 * width) + (coordinate.x * 3);

	color.x = array[pos];
	color.y = array[++pos];
	color.z = array[++pos];

}

void readConfig(bool &anti_alias, bool &gamma, bool &normal, float &light, int &width, int &height, Vec3<unsigned char> &gStart, Vec3<unsigned char> &gEnd, Vec3<float> &cameraPos) {

	// Read the ini settings file
	INIReader reader("../Config.ini");
	int temp0, temp1, temp2;

	anti_alias = reader.GetBoolean("settings", "anti-aliasing", false);
	gamma = reader.GetBoolean("settings", "gamma-correction", false);
	normal = reader.GetBoolean("settings", "normal-correction", false);
	light = (float)reader.GetReal("settings", "ambient-light", 0.2);
	width = reader.GetInteger("settings", "image-width", 512);
	height = reader.GetInteger("settings", "image-height", 512);

	temp0 = reader.GetInteger("Gradient", "start.r", -1);
	temp1 = reader.GetInteger("Gradient", "start.g", -1);
	temp2 = reader.GetInteger("Gradient", "start.b", -1);
	gStart.setValues(temp0, temp1, temp2);

	temp0 = reader.GetInteger("Gradient", "end.r", -1);
	temp1 = reader.GetInteger("Gradient", "end.g", -1);
	temp2 = reader.GetInteger("Gradient", "end.b", -1);
	gEnd.setValues(temp0, temp1, temp2);

	temp0 = reader.GetReal("Camera", "loc.x", -1);
	temp1 = reader.GetReal("Camera", "loc.y", -1);
	temp2 = reader.GetReal("Camera", "loc.z", -1);
	cameraPos.setValues(temp0, temp1, temp2);

}

int main(int argc, char * argv[]) {

	bool anti_aliasing, gamma_correction, normal_correction;
	float ambient_light;
	int width, height;
	Vec3<unsigned char> gradientStart(0, 0, 0), gradientEnd(0, 0, 0);
	Vec3<float> cameraPos(0, 0, 0);

	// Read the config setting
	readConfig(anti_aliasing, gamma_correction, normal_correction, ambient_light, width, height, gradientStart, gradientEnd, cameraPos);

	cout << "Anti-aliasing: " << anti_aliasing << endl;
	cout << "Gamma correction: " << gamma_correction << endl;
	cout << "Normal correction: " << normal_correction << endl;
	cout << "Ambient light value: " << ambient_light << endl;
	cout << "Image width: " << width << endl;
	cout << "Image height: " << height << endl;

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

	float pixelWidth = 1 / (float)width;
	float pixelHeight = 1 / (float)height;
	float truePixelHeightCenter, truePixelWidthCenter;
	if( width % 2 == 0 ) {
		truePixelWidthCenter = pixelWidth >> 2;
	}
	if( height % 2 == 0 ) {
		truePixelHeightCenter = pixelHeight >> 2;
	}

	float tempHeightOffset = truePixelHeightCenter;
	float tempWidthOffset  = truePixelWidthCenter;
	// Start going through all pixels and draw them
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {

			// Shoot five rays per pixel if anti-aliasing
			if(anti_aliasing) {

			} else {

			}
		}
	}
}
