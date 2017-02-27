#if defined(__linux) || defined(__APPLE__)
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
	#define HAVE_STRUCT_TIMESPEC // For pthread on windows VS2015
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION

/* Standard libs */
#include <iostream>
#include <pthread.h>
#include <vector>

/* Project libs */
#include "Color.hpp"
#include "Material.hpp"
#include "Point.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"

/* External libs*/
#include "INIReader.h"
#include "stb_image_write.h"
#include "tinyxml2.h"


using namespace std;

Color _ColorMapping(OBJECTS_FILE);

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

void initGeometry(std::vector<Geometry *> &geom, std::vector<Geometry *> &lights) {

	// Load the xml file
	tinyxml2::XMLDocument doc;
	doc.LoadFile(OBJECTS_FILE);

	// Check for errors within the file
	if (doc.Error()) {
		cout << "There was an error parsing " << OBJECTS_FILE << endl;
		doc.PrintError();
		exit(1);
	}
	// Go through the lighs array and geometry array
	while(geom.size() == 0 || lights.size() == 0) {

		// Grab the first child element in the file
		tinyxml2::XMLElement * objectParents = doc.FirstChildElement();
		int isObject = 0, isLight = 0;

		//Loop to find the objects or light parent element while the string is not "objects" or the size is non-zero
		while(objectParents && ((isObject = strncmp(objectParents->Value(), "objects", 7)) || geom.size() != 0) && ((isLight = strncmp(objectParents->Value(), "lights", 6)) || lights.size() != 0)) {
			//cout << "Element text " << objectParents->Value() << endl;
			objectParents = objectParents->NextSiblingElement();
		}
		isObject = !isObject;
		isLight = !isLight;

		if (!objectParents) {
			cout << "Failed to find the object/light portion in the xml file.  Exiting" << endl;
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
				Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
				Material mat = NONE;
				std::string str;

				// Go through and read all the attributes and tags
				tinyxml2::XMLElement * tag = objectParents->FirstChildElement();
				int vertexCount = 0;
				while (tag) {
					if (!strncmp(tag->Value(), "vertex", 6)) {

						// Read the 3 vectors' attributes and set their values
						double a = 0, b = 0, c = 0;
						tag->QueryDoubleAttribute("x", &a);
						tag->QueryDoubleAttribute("y", &b);
						tag->QueryDoubleAttribute("z", &c);

						// Set the vertex values
						if (vertexCount == 0) {
							vertexA.SetValues((float)a, (float)b, (float)c);
						}
						else if (vertexCount == 1) {
							vertexB.SetValues((float)a, (float)b, (float)c);
						}
						else {
							vertexC.SetValues((float)a, (float)b, (float)c);
						}
						vertexCount++;
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

				// Create a new triangle object and add it to the arrayj
				if (isObject) {
					geom.push_back(new Triangle(vertexA, vertexB, vertexC, color, mat));
				}
				else {
					lights.push_back(new Triangle(vertexA, vertexB, vertexC, color, mat));
				}


			} //Sphere object
			else if (!strncmp(objectParents->Value(), "sphere", 6)) {
				Vec3<float> center;
				float radius;
				Material mat = NONE;
				Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
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

				// Add the object to the light/geometry vector
				if (isObject) {
					geom.push_back(new Sphere(center, radius, color, mat));
				}
				else {
					lights.push_back(new Sphere(center, radius, color, mat));
				}
			}
			else if (!strncmp(objectParents->Value(), "point", 5)) {
				Vec3<float> point = Vec3<float>::vec3(0, 0, 0);

				// Go through and read all the attributes and tags
				tinyxml2::XMLElement * tag = objectParents->FirstChildElement();
				while (tag) {
					if (!strncmp(tag->Value(), "location", 6)) {
						double a, b, c;
						tag->QueryDoubleAttribute("x", &a);
						tag->QueryDoubleAttribute("y", &b);
						tag->QueryDoubleAttribute("z", &c);

						point.SetValues((float)a, (float)b, (float)c);
					}
					tag = tag->NextSiblingElement();
				}

				// Add the object to the light/geometry vector
				if (isObject) {
					geom.push_back(new Point(point));
				}
				else {
					lights.push_back(new Point(point));
				}
			}

			// Get the next object
			objectParents = objectParents->NextSiblingElement();
		}
	}
}

void gammaCorrect(unsigned char * imageArray, int height, int width) {
	Vec3<unsigned char> color;
	Vec2<int> coordinate;

	// Go through each of the pixels
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			//Corrected = 255 * (Image/255)^(1/2.2)
			coordinate.SetValues(j, i);
			getPixelColor(color, coordinate, imageArray, width);
			color.x = (unsigned char)(255 * pow((color.x / 255.f), 0.45454545454));
			color.z = (unsigned char)(255 * pow((color.z / 255.f), 0.45454545454));
			color.y = (unsigned char)(255 * pow((color.y / 255.f), 0.45454545454));

			setPixelColor(color, coordinate, imageArray, width);
		}
	}
}

void DestroyGeometry(std::vector<Geometry *> &geom) {
	for (std::vector<Geometry *>::size_type i = 0; i != geom.size(); i++) {
		if (geom[i] != NULL) {
			delete(geom[i]);
		}
	}
}

Vec3<float> GetReflection(Vec3<float> ray, Vec3<float> norm) {
	float temp = 2 * (ray * norm);
	return Vec3<float>::Normalize(ray - (norm * temp));
}

std::shared_ptr<RayHit> GetRay(Vec3<float> ray, Vec3<float> startingPos, vector<Geometry *> &geom, int depth) {
	
	float time = -1;
	shared_ptr<RayHit> minHit = nullptr;
	//cout << "Ray is " << ray.x << " " << ray.y << " " << ray.z << endl;

	for (size_t i = 0; i < geom.size(); i++) {
		shared_ptr<RayHit> rayHit = geom.at(i)->Intersect(ray, startingPos);
		if(rayHit != nullptr) {
			if(time < 0 || rayHit->GetTime() < time) {
				time = rayHit->GetTime();
				minHit = rayHit;
			}
		} 
	}
	if(minHit == nullptr) {
		return nullptr;
	}

	/* Check reflection */
	if(minHit->GetMaterial() == REFLECTIVE) {
		if(depth > 9) {
			return nullptr;
		}
		return GetRay(GetReflection(minHit->GetRay(), minHit->GetNormal()), minHit->GetHitLocation() + (minHit->GetNormal() * .00005f), geom, depth+1);
	}
	return minHit;
}

Vec3<unsigned char> CheckShadows(float ambientLight, std::shared_ptr<RayHit> rayHit, vector<Geometry *> &geometry, vector<Geometry *> &lights) {
	
	bool intersected = false;
	float scale = ambientLight;

	// Go through each light source
	for (size_t i = 0; i < lights.size(); i++) {
		Vec3<float> randomPoint = lights.at(i)->GetRandomPoint();
		Vec3<float> toLightRay = Vec3<float>::Normalize(randomPoint - (rayHit->GetHitLocation() + (rayHit->GetNormal() * .00005f)) ); // Bump

		// See if the ray from the light source is in shadow or figure out the dot product between the two
		for (size_t j = 0; j < geometry.size(); j++) {
			std::shared_ptr<RayHit> tempHit;
			if ((tempHit = geometry.at(j)->Intersect(toLightRay, rayHit->GetHitLocation())) != nullptr) {

				//Make sure we didn't hit anything behind us
				if (tempHit->GetTime() > 0.0005f) {
					intersected = true;
				}
			}
		}

		// We didn't hit anything so take the dot product
		if (!intersected) {
			float temp = toLightRay * rayHit->GetNormal();

			// Ambient light calculation
			if (temp > scale) {
				scale = temp;
			}
		}
	}
	
	return rayHit->GetColor() * scale;
}

int main(int argc, char * argv[]) {

	bool anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation;
	float ambient_light, plane_width, plane_height;
	int image_width, image_height;
	Vec3<float> gradientStart(0, 0, 0), gradientEnd(0, 0, 0), cameraPos(0, -.25, 0);
	float imagePlaneWidth = -2;
	std::vector<Geometry *> geometryArray;
	std::vector<Geometry *> lightArray;

	// Read the config setting
	readConfig(anti_aliasing, gamma_correction, normal_correction, background_gradient, hsl_interpolation, ambient_light, image_width, image_height, gradientStart, gradientEnd);
	initGeometry(geometryArray, lightArray);
	

	cout << "Anti-aliasing: " << anti_aliasing << endl;
	cout << "Gamma correction: " << gamma_correction << endl;
	cout << "Normal correction: " << normal_correction << endl;
	cout << "Ambient light value: " << ambient_light << endl;
	cout << "Image width: " << image_width << endl;
	cout << "Image height: " << image_height << endl;

	unsigned char * imageArray = (unsigned char *) malloc(3 * image_width * image_height * sizeof(unsigned char));

	if(!imageArray) {
		cout << "Failed to allocate memory for the image array.  Exiting" << endl;
		exit(1);
	}

	// Draw the gradient on the image
	if(background_gradient) {
		drawGradient(gradientStart, gradientEnd, image_width, image_height, imageArray, hsl_interpolation);
	}

	//Plane width (always 2 in the greatest direction)
	if( image_width > image_height ) {
		plane_width = 2.f;
		plane_height = 2 * (image_height / (float) image_width);
	} else {
		plane_height = 2.f;
		plane_width = 2 * (image_width / (float) image_height);
	}

	// Pixel height calculations
	float pixel_width = plane_width / (float)image_width;
	float pixel_height = plane_height / (float)image_height;

	float width_offset = image_width % 2 == 0 ? pixel_width / 2.f : 0.f;
	float height_offset = image_height % 2 == 0 ? pixel_height / 2.f : 0.f;

	/*Iterate through the geometry 
	for (size_t i = 0; i < geometryArray.size(); i++) {
		cout << "Geometry number " << i+1 << endl;
		cout << "Shape " << geometryArray[i]->GetShape() << endl;
		cout << "Material " << geometryArray[i]->GetMaterial() << endl;
		cout << "Color " << (int)geometryArray[i]->GetColor().x;
		cout << " " << (int)geometryArray[i]->GetColor().y;
		cout << " " << (int)geometryArray[i]->GetColor().z << endl;
	}

	//Iterate through the lights
	for (size_t i = 0; i < lightArray.size(); i++) {
		cout << "Light number " << i + 1 << endl;
		cout << "Shape " << lightArray[i]->GetShape() << endl;
		cout << "Color " << (int)lightArray[i]->GetColor().x;
		cout << " " << (int)lightArray[i]->GetColor().y;
		cout << " " << (int)lightArray[i]->GetColor().z << endl;
	}*/

	// Start going through all pixels and draw them
	for(int i = 0; i < image_height; i++) {
		float pixel_center_height = (plane_height / 2.f - i * pixel_height - height_offset);
		for(int j = 0; j < image_width; j++) {
			float pixel_center_width = (plane_width / -2.f + j * pixel_width + width_offset);
			// Shoot five rays per pixel if anti-aliasing
			if(anti_aliasing) {
				for(int k = 0; k < 4; k++) {

				}
			} else {
				//Shoot a single ray 
				//cout << "Shooting pixel to " << pixel_center_width << " height " << pixel_center_height << " -2" << endl;
				Vec3<float> tempRay = Vec3<float>::Normalize(Vec3<float>::vec3(pixel_center_width, pixel_center_height, imagePlaneWidth) - cameraPos);
				std::shared_ptr<RayHit> rayHit = GetRay(tempRay, cameraPos, geometryArray, 0);
				Vec3<unsigned char> color = rayHit == nullptr ? _ColorMapping.GetColor("BLACK") : rayHit->GetColor();
				Vec2<int> coord(j, i);

				// Set the pixel color
				if (rayHit == nullptr) {
					setPixelColor(_ColorMapping.GetColor("BLACK"), coord, imageArray, image_width);
				}
				else {
					Vec3<unsigned char> color = CheckShadows(ambient_light, rayHit, geometryArray, lightArray);
					setPixelColor(color, coord, imageArray, image_width);
				}
			}
		}
	}

	// Gamma correction
	if(gamma_correction) {
		gammaCorrect(imageArray, image_height, image_width);
	}

	// Write out the image
	stbi_write_png("output.png", image_width, image_height, 3, imageArray, image_width*3);
	DestroyGeometry(geometryArray);
	DestroyGeometry(lightArray);
	free(imageArray);
}
