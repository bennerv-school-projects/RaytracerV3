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
	#define OBJECTS_FILE "../Objects.xml"
	#define CONFIG_FILE "../Config.ini"
#else 
	#define OBJECTS_FILE "Objects.xml"
	#define CONFIG_FILE "Config.ini"
#endif

/* Standard libs */
#include <iostream>
#include <typeinfo>
#include <vector>

/* Project libs */
#include "Color.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"

/* External libs*/
#include "INIReader.h"
#include "stb_image_write.h"
#include "tinyxml2.h"

/* Additional definitions */
#define STB_IMAGE_WRITE_IMPLEMENTATION 

using namespace std;

Color _ColorMapping(OBJECTS_FILE);

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

void readConfig(bool &anti_alias, bool &gamma, bool &normal, bool &gradient, bool &hsl, float &light, int &width, int &height, Vec3<float> &gStart, Vec3<float> &gEnd) {

	// Read the ini settings file
	INIReader reader(CONFIG_FILE);

	anti_alias = reader.GetBoolean("settings", "anti-aliasing", false);
	gamma = reader.GetBoolean("settings", "gamma-correction", false);
	normal = reader.GetBoolean("settings", "normal-correction", false);
	gradient = reader.GetBoolean("settings", "background-gradient", false);
	hsl = reader.GetBoolean("settings", "hsl-interpolation", false);
	light = (float)reader.GetReal("settings", "ambient-light", 0.2);
	width = reader.GetInteger("settings", "image-width", 512);
	height = reader.GetInteger("settings", "image-height", 512);

	if (gradient) {
		std::string str("RED"), temp;
		Vec3<unsigned char> charColor;
		temp = reader.Get("Gradient", "start", str);
		charColor = _ColorMapping.GetColor(temp);
		gStart = Vec3<float>::vec3(charColor.x, charColor.y, charColor.z);

		temp = reader.Get("Gradient", "end", str);
		charColor = _ColorMapping.GetColor(temp);
		gEnd = Vec3<float>::vec3(charColor.x, charColor.y, charColor.z);

	}
}

void drawGradient(Vec3<float> gradientStart, Vec3<float> gradientEnd, int width, int height, unsigned char * imageArray, bool hsl) {
	float t;

	// HSL gradient
	if (hsl) {
		Color::RGBToHSL(gradientStart);
		Color::RGBToHSL(gradientEnd);
	}
	for (int i = 0; i < height; i++) {
		t = i / (float)(height - 1.0);
		float r = gradientStart.x + (gradientEnd.x - gradientStart.x) * t;
		float g = gradientStart.y + (gradientEnd.y - gradientStart.y) * t;
		float b = gradientStart.z + (gradientEnd.z - gradientStart.z) * t;
		Vec3<float> color(r, g, b);

		// Convert the hsl interpolation to rgb if necessary so we can draw it
		if (hsl) {
			Color::HSLToRGB(color);
		}

		Vec3<unsigned char> col((unsigned char)color.x, (unsigned char)color.y, (unsigned char)color.z);

		for (int j = 0; j < width; j++) {
			// a + (b - a) * t
			Vec2<int> coordinate(i, j);
			setPixelColor(col, coordinate, imageArray, width);
		}
	}
}

void initGeometry(std::vector<Geometry *> &geom) {

	// Load the xml file
	tinyxml2::XMLDocument doc;
	doc.LoadFile(OBJECTS_FILE);

	// Check for errors within the file
	if (doc.Error()) {
		cout << "There was an error parsing " << OBJECTS_FILE << endl;
		doc.PrintError();
		exit(1);
	}

	// Grab the first child element in the file
	tinyxml2::XMLElement * objectParents = doc.FirstChildElement();
	while (objectParents && strncmp(objectParents->Value(), "objects", 7)) {
		//cout << "Element text " << colorParent->Value() << endl;
		objectParents = objectParents->NextSiblingElement();
	}

	if (!objectParents) {
		cout << "Failed to find the object portion in the xml file.  Exiting" << endl;
		exit(10);
	}

	// Iterate through the objects portion and add them to the geometry array
	objectParents = objectParents->FirstChildElement();
	while (objectParents) {

		// Triangle object
		if (!strncmp(objectParents->Value(), "triangle", 8)) {
			Vec3<float> vertexA;
			Vec3<float> vertexB;
			Vec3<float> vertexC;
			Vec3<unsigned char> color;
			Material mat = NONE;
			std::string str;

			// Go through and read all the attributes and tags
			tinyxml2::XMLElement * tag = objectParents->FirstChildElement();
			while (tag) {
				if (!strncmp(tag->Value(), "vertex", 6)) {

					// Read the 3 vectors' attributes and set their values
					char * temp = (char *)tag->Attribute("name");
					double a = 0, b = 0, c = 0;
					tag->QueryDoubleAttribute("x", &a);
					tag->QueryDoubleAttribute("y", &b);
					tag->QueryDoubleAttribute("z", &c);

					// Switch at each of the vectors
					switch (temp[0]) {
						case 'A' :
							vertexA.SetValues((float)a, (float)b, (float)c);
							break;
						case 'B' :
							vertexB.SetValues((float)a, (float)b, (float)c);
							break;
						case 'C' :
							vertexC.SetValues((float)a, (float)b, (float)c);
							break;
						default :
							break;
					}
				}
				else if (!strncmp(tag->Value(), "color", 5)) {

					// Read the color and set the corresponding triangle color
					str.assign(tag->GetText());
					std::transform(str.begin(), str.end(), str.begin(), ::toupper);
					color = _ColorMapping.GetColor(str);

				}
				else if (!strncmp(tag->Value(), "material", 8)) {

					// Read the material and set the corresponding material for the triangle
					str.assign(tag->GetText());
					std::transform(str.begin(), str.end(), str.begin(), ::toupper);
					
					// Assign the material
					if (!strncmp(str.c_str(), "NONE", 4)) {
						mat = NONE;
					}
					else if (!strncmp(str.c_str(), "REFLECTIVE", 10)) {
						mat = REFLECTIVE;
					}
					else if (!strncmp(str.c_str(), "SPECULAR", 8)) {
						mat = SPECULAR;
					}
					else if (!strncmp(str.c_str(), "GLASS", 5)) {
						mat = GLASS;
					}
				}
				tag = tag->NextSiblingElement();
			}

			// Create a new triangle object and add it to the array
			geom.push_back(new Triangle(vertexA, vertexB, vertexC, color, mat));
		
		// Sphere object
		} else if (!strncmp(objectParents->Value(), "sphere", 6)) {
			Vec3<float> center;
			float radius;
			Material mat = NONE;
			Vec3<unsigned char> color;
			std::string str;

			// Go through and read all the attributes and tags
			tinyxml2::XMLElement * tag = objectParents->FirstChildElement();
			while (tag) {
				if (!strncmp(tag->Value(), "center", 6)) {
					double a, b, c;
					tag->QueryDoubleAttribute("x", &a);
					tag->QueryDoubleAttribute("y", &b);
					tag->QueryDoubleAttribute("z", &c);

					center.SetValues((float)a, (float)b, (float)c);
				}
				else if (!strncmp(tag->Value(), "radius", 6)) {

					// Read the radius
					radius = (float)atof(tag->GetText());
				}
				else if (!strncmp(tag->Value(), "color", 5)) {

					// Read the color and set the corresponding triangle color
					str.assign(tag->GetText());
					std::transform(str.begin(), str.end(), str.begin(), ::toupper);
					color = _ColorMapping.GetColor(str);

				}
				else if (!strncmp(tag->Value(), "material", 8)) {

					// Read the material and set the corresponding material for the triangle
					str.assign(tag->GetText());
					std::transform(str.begin(), str.end(), str.begin(), ::toupper);

					// Assign the material
					if (!strncmp(str.c_str(), "NONE", 4)) {
						mat = NONE;
					}
					else if (!strncmp(str.c_str(), "REFLECTIVE", 10)) {
						mat = REFLECTIVE;
					}
					else if (!strncmp(str.c_str(), "SPECULAR", 8)) {
						mat = SPECULAR;
					}
					else if (!strncmp(str.c_str(), "GLASS", 5)) {
						mat = GLASS;
					}
				}
				tag = tag->NextSiblingElement();
			}

			// Add the object to the geometry vector/array
			geom.push_back(new Sphere(center, radius, color, mat));
		}

		// Get the next object
		objectParents = objectParents->NextSiblingElement();
	}
}

void gammaCorrect(unsigned char * imageArray, int height, int width) {
	Vec3<unsigned char> color;
	Vec2<int> coordinate;

	// Go through each of the pixels
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			//Corrected = 255 * (Image/255)^(1/2.2)
			coordinate.SetValues(i, j);
			getPixelColor(color, coordinate, imageArray, width);
			color.x = (unsigned char)(255 * pow((color.x / 255.f), 0.45454545454));
			color.z = (unsigned char)(255 * pow((color.z / 255.f), 0.45454545454));
			color.y = (unsigned char)(255 * pow((color.y / 255.f), 0.45454545454));

			setPixelColor(color, coordinate, imageArray, width);
		}
	}
}

void destroyGeometry(std::vector<Geometry *> &geom) {
	for (std::vector<Geometry *>::size_type i = 0; i != geom.size(); i++) {
		if (geom[i] != NULL) {
			delete(geom[i]);
		}
	}
}

int main(int argc, char * argv[]) {

	bool anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation;
	float ambient_light;
	int width, height;
	Vec3<float> gradientStart(0, 0, 0), gradientEnd(0, 0, 0);
	std::vector<Geometry *> geometryArray;

	// Read the config setting
	readConfig(anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation, ambient_light, width, height, gradientStart, gradientEnd);
	initGeometry(geometryArray);

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
	if(background_gradient) {
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

	for (int i = 0; i < geometryArray.size(); i++) {
		const char * name = typeid(geometryArray[i]).name();
		cout << name << endl;
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
		gammaCorrect(imageArray, height, width);
	}

	// Write out the image
	//stbi_write_png("output.png", width, height, 3, imageArray, width*3);
	destroyGeometry(geometryArray);
	free(imageArray);
}
