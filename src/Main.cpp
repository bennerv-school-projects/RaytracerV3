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

#include <iostream>

#include "INIReader.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "Vector.hpp"

using namespace std;

void setPixelColor(Vec3<unsigned char> color, Vec2<int> coordinate, unsigned char * array, int width) {

	int pos = (coordinate.x * 3 * width) + (coordinate.y * 3);

	array[pos] = color.x;
	array[++pos] = color.y;
	array[++pos] = color.z;
}

void getPixelColor(Vec3<unsigned char> &color, Vec2<int> coordinate, unsigned char * array, int width) {
	int pos = (coordinate.x * 3 * width) + (coordinate.y * 3);

	color.x = array[pos];
	color.y = array[++pos];
	color.z = array[++pos];

}

void RGBToHSL(Vec3<float> &color) {
	float r = color.x / 255.;
	float g = color.y / 255.;
	float b = color.z / 255.;

	char maxSel;

	float cmax, cmin, delta;

	float H, S, L;

	if( r >= g && r >= b) {
		maxSel = 'r';
		cmax = r;
	} else if( g >= r && g >= b) {
		maxSel = 'g';
		cmax = g;
	} else {
		maxSel = 'b';
		cmax = b;
	}

	cmin = fminf(fminf(r, g), b);
	delta = cmax - cmin;

	L = (cmax + cmin) / 2.;

	if(delta == 0) {
		H = 0.;
		S = 0.;
	} else {
		switch(maxSel) {
			case 'r':
				H = 60 * fmodf((g - b) / delta, 6);
				break;
			case 'g':
				H = 60 * (((b - r) / delta) + 2);
				break;
			case 'b' :
				H = 60 * (((r - g) / delta) + 4);
				break;
			default :
				H = 0;
				break;
		}
		float temp = 2 * L - 1;
		if( temp < 0 ) {
			temp *= -1;
		}
		S = 100. * delta / (1. - temp);
	}
	color.setValues(H, S, 100*L);

}

void HSLToRGB(Vec3<float> &color) {

	float H = color.x;
	float S = color.y / 100.;
	float L = color.z / 100.;

	float C = (1.0 - abs(2 * L - 1)) * S;

	float X = C * (1 - abs(fmodf(H / 60, 2) - 1));

	float m = L - (C / 2.);

	float r, g, b;
	if(H >= 300) {
		r = C;
		g = 0;
		b = X;
	} else if(H >= 240) {
		r = X;
		g = 0;
		b = C;
	} else if(H >= 180) {
		r = 0;
		g = X;
		b = C;
	} else if(H >= 120) {
		r = 0;
		g = C;
		b = X;
	} else if(H >= 60) {
		r = X;
		g = C;
		b = 0;
	} else {
		r = C;
		g = X;
		b = 0;
	}

	color.setValues(255. * (r + m), 255. * (g + m), 255. * (b + m));


}

void readConfig(bool &anti_alias, bool &gamma, bool &normal, bool &gradient, bool &hsl, float &light, int &width, int &height, Vec3<float> &gStart, Vec3<float> &gEnd, Vec3<float> &cameraPos) {

	// Read the ini settings file
	INIReader reader("../Config.ini");
	int temp0, temp1, temp2;

	anti_alias = reader.GetBoolean("settings", "anti-aliasing", false);
	gamma = reader.GetBoolean("settings", "gamma-correction", false);
	normal = reader.GetBoolean("settings", "normal-correction", false);
	gradient = reader.GetBoolean("settings", "background-gradient", false);
	hsl = reader.GetBoolean("settings", "hsl-interpolation", false);
	light = (float)reader.GetReal("settings", "ambient-light", 0.2);
	width = reader.GetInteger("settings", "image-width", 512);
	height = reader.GetInteger("settings", "image-height", 512);

	if( gradient ) {
		temp0 = reader.GetInteger("Gradient", "start.r", -1);
		temp1 = reader.GetInteger("Gradient", "start.g", -1);
		temp2 = reader.GetInteger("Gradient", "start.b", -1);
		gStart.setValues(temp0, temp1, temp2);

		temp0 = reader.GetInteger("Gradient", "end.r", -1);
		temp1 = reader.GetInteger("Gradient", "end.g", -1);
		temp2 = reader.GetInteger("Gradient", "end.b", -1);
		gEnd.setValues(temp0, temp1, temp2);
	}

	temp0 = reader.GetReal("Camera", "loc.x", -1);
	temp1 = reader.GetReal("Camera", "loc.y", -1);
	temp2 = reader.GetReal("Camera", "loc.z", -1);
	cameraPos.setValues(temp0, temp1, temp2);

}

void drawGradient(Vec3<float> gradientStart, Vec3<float> gradientEnd, int width, int height, unsigned char * imageArray, bool hsl) {
	float t;

	// HSL gradient
	if(hsl) {
		RGBToHSL(gradientStart);
		RGBToHSL(gradientEnd);
	}
	for(int i = 0; i < height; i++) {
		t = i / (float)(height - 1.0);
		float r = gradientStart.x + (gradientEnd.x - gradientStart.x) * t;
		float g = gradientStart.y + (gradientEnd.y - gradientStart.y) * t;
		float b = gradientStart.z + (gradientEnd.z - gradientStart.z) * t;
		Vec3<float> color(r, g, b);

		// Convert the hsl interpolation to rgb if necessary so we can draw it
		if( hsl ) {
			HSLToRGB(color);
		}

		Vec3<unsigned char> col(color.x, color.y, color.z);

		for(int j = 0; j < width; j++) {
			// a + (b - a) * t
			Vec2<int> coordinate(i, j);
			setPixelColor(col, coordinate, imageArray, width);
		}
	}
}



int main(int argc, char * argv[]) {

	bool anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation;
	float ambient_light;
	int width, height;
	Vec3<float> gradientStart(0, 0, 0), gradientEnd(0, 0, 0);
	Vec3<float> cameraPos(0, 0, 0);

	// Read the config setting
	readConfig(anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation, ambient_light, width, height, gradientStart, gradientEnd, cameraPos);

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

	// Draw the gradient on the image
	if( background_gradient) {
		drawGradient(gradientStart, gradientEnd, width, height, imageArray, hsl_interpolation);
	}

	// Pixel coordinate correction
	float pixelWidth = 1 / (float)width;
	float pixelHeight = 1 / (float)height;
	float truePixelHeightCenter = 0, truePixelWidthCenter = 0;
	if( width % 2 == 0 ) {
		truePixelWidthCenter = pixelWidth / 2;
	}
	if( height % 2 == 0 ) {
		truePixelHeightCenter = pixelHeight / 2;
	}

	float tempHeightOffset = truePixelHeightCenter;
	float tempWidthOffset  = truePixelWidthCenter;

	if(anti_aliasing) {
		tempHeightOffset -= pixelHeight / 4;
		tempWidthOffset -= pixelWidth / 4;
	}

	// Start going through all pixels and draw them
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {

			// Shoot five rays per pixel if anti-aliasing
			if(anti_aliasing) {
				for(int k = 0; k < 4; k++) {

				}
			} else {
				//Shoot a single ray 
			}
		}
	}

	// Gamma correction
	if(gamma_correction) {
		Vec3<unsigned char> color;
		Vec2<int> coordinate;

		// Go through each of the pixels
		for(int i = 0; i < height; i++) {
			for(int j = 0; j < width; j++) {

				//Corrected = 255 * (Image/255)^(1/2.2)
				coordinate.setValues(i, j);
				getPixelColor(color, coordinate, imageArray, width);
				color.x = 255 * pow((color.x / (float)255), 0.45454545454);
				color.y = 255 * pow((color.y / (float)255), 0.45454545454);
				color.z = 255 * pow((color.z / (float)255), 0.45454545454);

				setPixelColor(color, coordinate, imageArray, width);
			}
		}
	}

	// Write out the image
	stbi_write_png("output.png", width, height, 3, imageArray, width*3);
	free(imageArray);
}
