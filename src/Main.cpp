#if defined(__linux) || defined(__APPLE__)
	#define OBJECTS_FILE "../Objects.xml"
	#define CONFIG_FILE "../Config.ini"
#else 
	#define OBJECTS_FILE "Objects.xml"
	#define CONFIG_FILE "Config.ini"
	#define HAVE_STRUCT_TIMESPEC // For pthread on windows VS2015
	#define PTW32_STATIC_LIB 
#endif

/* STB Image write definition needed for writing png file */
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define MAX_THREADS 20 // Must be at least 1

/* Standard libs */
#include <cassert>
#include <iostream>
#include <pthread.h>
#include <vector>

/* Project libs */
#include "Color.hpp"
#include "Material.hpp"
#include "Perspective.hpp"
#include "Point.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"

/* External libs*/
#include "INIReader.h"
#include "stb_image_write.h"
#include "tinyxml2.h"

typedef struct {
    int threadId;
    Perspective * perspective;
    std::vector<Geometry *> * geometryArray;
    std::vector<Geometry *> * lightArray;
    unsigned char * imageArray;
    
    
} threadArgs;


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

void readConfig(Perspective * perspective, bool &gamma, bool &normal, bool &gradient, bool &hsl, Vec3<float> &gStart, Vec3<float> &gEnd) {

	// Read the ini settings file
	INIReader reader(CONFIG_FILE);

    perspective->SetAntiAliasing(reader.GetBoolean("settings", "anti-aliasing", false));
	gamma = reader.GetBoolean("settings", "gamma-correction", false);
	normal = reader.GetBoolean("settings", "normal-correction", false);
	gradient = reader.GetBoolean("settings", "background-gradient", false);
	hsl = reader.GetBoolean("settings", "hsl-interpolation", false);
    perspective->SetAmbientLight(reader.GetReal("settings", "ambient-light", 0.2f));
	perspective->SetPixelLength((int)reader.GetInteger("settings", "image-length", 512));
    perspective->SetPixelHeight((int)reader.GetInteger("settings", "image-height", 512));

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
		t = i / (float)(height - 1.0f);
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

void initGeometry(std::vector<Geometry *> &geom, std::vector<Geometry *> &lights, Perspective * perspective) {

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
    
	// Go through the lights array and geometry array
	while(objectParents) {
        
		int isObject = 1, isCamera = 1, isImagePlane = 1;

		//Loop to find the objects or light parent element while the string is not "objects" or the size is non-zero
		while(objectParents && (isObject = strncmp(objectParents->Value(), "objects", 7)) && (strncmp(objectParents->Value(), "lights", 6)) && (isCamera = strncmp(objectParents->Value(), "camera", 6)) && (isImagePlane = strncmp(objectParents->Value(), "image_plane", 11)) ) {
			//cout << "Element text " << objectParents->Value() << endl;
			objectParents = objectParents->NextSiblingElement();
		}
		isObject = !isObject;
        isCamera = !isCamera;
        isImagePlane = !isImagePlane;

		if (!objectParents) {
            break;
		}
        
        
        tinyxml2::XMLElement * objectChild = objectParents->FirstChildElement();
        
        // Camera parsing
        if(isCamera) {
            if (!strncmp(objectChild->Value(), "location", 8)) {
                
                // Read the 3 vectors' attributes and set their values
                double a = 0, b = 0, c = 0;
                objectChild->QueryDoubleAttribute("x", &a);
                objectChild->QueryDoubleAttribute("y", &b);
                objectChild->QueryDoubleAttribute("z", &c);
                
                perspective->SetCameraPosition(Vec3<float>::vec3(a, b, c));
            }
        } else if(isImagePlane) { // imagePlane parsing
            float length = 0, width = 0, height = 0;
            Vec3<float> corner;
            
            // Get all the attributes for the ImagePlane
            while(objectChild) {
                if(!strncmp(objectChild->Value(), "corner", 6)) {
                    double a = 0, b = 0, c = 0;
                    objectChild->QueryDoubleAttribute("x", &a);
                    objectChild->QueryDoubleAttribute("y", &b);
                    objectChild->QueryDoubleAttribute("z", &c);
                    
                    corner = Vec3<float>::vec3(a, b, c);
                    
                } else if(!strncmp(objectChild->Value(), "length", 6)) {
                    length = (float)atof(objectChild->GetText());
                } else if(!strncmp(objectChild->Value(), "width", 5)) {
                    width = (float)atof(objectChild->GetText());
                } else if(!strncmp(objectChild->Value(), "height", 6)) {
                    height = (float)atof(objectChild->GetText());
                }
                
                objectChild = objectChild->NextSiblingElement();
            }
            
            perspective->SetImagePlane(new ImagePlane(corner, length, width, height));
            
        } else { // object/light parsing

            // Iterate through the objects portion and add them to the geometry array
            while (objectChild) {

                // Triangle object
                if (!strncmp(objectChild->Value(), "triangle", 8)) {
                    Vec3<float> vertexA;
                    Vec3<float> vertexB;
                    Vec3<float> vertexC;
                    Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
                    Material mat = NONE;
                    std::string str;

                    // Go through and read all the attributes and tags
                    tinyxml2::XMLElement * tag = objectChild->FirstChildElement();
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
                else if (!strncmp(objectChild->Value(), "sphere", 6)) {
                    Vec3<float> center(0, 0, 0);
                    float radius = 0;
                    Material mat = NONE;
                    Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
                    std::string str;

                    // Go through and read all the attributes and tags
                    tinyxml2::XMLElement * tag = objectChild->FirstChildElement();
                    while (tag) {
                        if (!strncmp(tag->Value(), "center", 6)) {
                            double a = 0, b = 0, c = 0;
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
                else if (!strncmp(objectChild->Value(), "point", 5)) {
                    Vec3<float> point = Vec3<float>::vec3(0, 0, 0);

                    // Go through and read all the attributes and tags
                    tinyxml2::XMLElement * tag = objectChild->FirstChildElement();
                    while (tag) {
                        if (!strncmp(tag->Value(), "location", 6)) {
                            double a = 0, b = 0, c = 0;
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
                objectChild = objectChild->NextSiblingElement();
            }
        }
        
        // Next sibling element
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
	for (std::vector<Geometry *>::size_type i = 0; i < geom.size(); i++) {
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

// pthreading shooting a single pixel per coordinate
void *ShootRays(void * arg) {

    threadArgs args;
    args = *((threadArgs *) arg);
    // Start going through all pixels and draw them
    for(int i = args.threadId; i < args.perspective->GetPixelHeight(); i+=MAX_THREADS) {
        Vec3<float> heightOffset = args.perspective->GetImagePlane()->GetCorner() - (args.perspective->GetUnitsPerHeightPixel() * i);
        
        for(int j = 0; j < args.perspective->GetPixelLength(); j++) {
            Vec3<float> trueOffset = heightOffset + (args.perspective->GetUnitsPerLengthPixel() * j);

            // Anti-aliasing
            if(args.perspective->GetAntiAliasing()) {
                std::vector<Vec3<unsigned char> > colorArray;
                for(int k = 0; k < 2; k++) {
                    Vec3<float> aliasHeightOffset = trueOffset - (args.perspective->GetUnitsPerHeightPixel() * (k+1) );
					for (int l = 0; l < 2; l++) {
                        Vec3<float> aliasTotalOffset = aliasHeightOffset + (args.perspective->GetUnitsPerLengthPixel() * l);
                        Vec3<float> tempRay = Vec3<float>::Normalize(aliasTotalOffset - args.perspective->GetCameraPosition());
                        std::shared_ptr<RayHit> rayHit = GetRay(tempRay, args.perspective->GetCameraPosition(), *(args.geometryArray), 0);
                        
                        // Push the colors back in the vector
                        if(rayHit == nullptr) {
                            colorArray.push_back(_ColorMapping.GetColor("BLACK"));
                        } else {
                            colorArray.push_back(CheckShadows(args.perspective->GetAmbientLight(), rayHit, *(args.geometryArray), *(args.lightArray)));
                        }
					}
                }
                
                Vec2<int> coord(j, i);
                // find the average between the four colored rays
                int avg [3] = {0};
                for(size_t size = 0; size < colorArray.size(); size++) {
                    avg[0] += colorArray.at(size).x;
                    avg[1] += colorArray.at(size).y;
                    avg[2] += colorArray.at(size).z;
                }
                
                Vec3<unsigned char> colorAvg(avg[0] / colorArray.size(), avg[1] / colorArray.size(), avg[2] / colorArray.size());
                setPixelColor(colorAvg, coord, args.imageArray, args.perspective->GetPixelLength());
                
            } else {
                //Shoot a single ray
                Vec3<float> tempRay = Vec3<float>::Normalize(trueOffset - args.perspective->GetCameraPosition());
                std::shared_ptr<RayHit> rayHit = GetRay(tempRay, args.perspective->GetCameraPosition(), *(args.geometryArray), 0);
                Vec2<int> coord(j, i);
                
                // Set the pixel color
                if (rayHit == nullptr) {
                    setPixelColor(_ColorMapping.GetColor("BLACK"), coord, args.imageArray, args.perspective->GetPixelLength());
                } else {
                    Vec3<unsigned char> color = CheckShadows(args.perspective->GetAmbientLight(), rayHit, *(args.geometryArray), *(args.lightArray));
                    setPixelColor(color, coord, args.imageArray, args.perspective->GetPixelLength());
                }
            }
        }
    }
    pthread_exit(NULL);
	return NULL;
}

int main(int argc, char * argv[]) {

    pthread_t pThreads[MAX_THREADS];
    Perspective * perspective = new Perspective();
	bool gamma_correction, normal_correction, background_gradient, hsl_interpolation;
	Vec3<float> gradientStart(0, 0, 0), gradientEnd(0, 0, 0);
	std::vector<Geometry *> geometryArray;
	std::vector<Geometry *> lightArray;

	// Read the config setting
	readConfig(perspective, gamma_correction, normal_correction, background_gradient, hsl_interpolation, gradientStart, gradientEnd);
	initGeometry(geometryArray, lightArray, perspective);
	
    /*
	cout << "Anti-aliasing: " << perspective->GetAntiAliasing() << endl;
	cout << "Gamma correction: " << gamma_correction << endl;
	cout << "Normal correction: " << normal_correction << endl;
	cout << "Ambient light value: " << perspective->GetAmbientLight() << endl;
	cout << "Image length: " << perspective->GetPixelLength() << endl;
	cout << "Image height: " << perspective->GetPixelHeight() << endl;
     */

	unsigned char * imageArray0 = (unsigned char *) malloc(3 * perspective->GetPixelLength() * perspective->GetPixelHeight() * sizeof(unsigned char));
    unsigned char * imageArray1 = (unsigned char *) malloc(3 * perspective->GetPixelLength() * perspective->GetPixelHeight() * sizeof(unsigned char));

	if(!imageArray0) {
		cout << "Failed to allocate memory for the image array.  Exiting" << endl;
		exit(1);
	}

	// Draw the gradient on the image
	if(background_gradient) {
		drawGradient(gradientStart, gradientEnd, perspective->GetPixelLength(), perspective->GetPixelHeight(), imageArray0, hsl_interpolation);
	}
    
    // Make sure the ImagePlane is set already
    assert(perspective->GetImagePlane() != nullptr);
    
    /* SEND PTHREADS AND SHIT */
    threadArgs * tArgs = (threadArgs *) malloc(sizeof(threadArgs) * MAX_THREADS);
    tArgs[0].geometryArray = &geometryArray;
    tArgs[0].imageArray = imageArray0;
    tArgs[0].lightArray = &lightArray;
    tArgs[0].perspective = perspective;
    tArgs[0].threadId = 0;

    for(int i = 0; i < MAX_THREADS; i++) {
        std::memcpy(&tArgs[i], &tArgs[0], sizeof(threadArgs));
    }
    
    for(int i = 0; i < MAX_THREADS; i++) {
        tArgs[i].threadId = i;
        pthread_create(&pThreads[i], NULL, ShootRays, &tArgs[i]);
    }
    
    // Wait for the threads
    for(int i = 0; i < MAX_THREADS; i++) {
        pthread_join(pThreads[i], NULL);
    }
    
	// Gamma correction
	if(gamma_correction) {
		gammaCorrect(imageArray0, perspective->GetPixelHeight(), perspective->GetPixelLength());
	}

	// Write out the image
	stbi_write_png("output.png", perspective->GetPixelLength(), perspective->GetPixelHeight(), 3, imageArray0, perspective->GetPixelLength()*3);
	DestroyGeometry(geometryArray);
	DestroyGeometry(lightArray);
    delete(perspective);
	free(imageArray0);
    free(imageArray1);
	pthread_exit(NULL);
}
