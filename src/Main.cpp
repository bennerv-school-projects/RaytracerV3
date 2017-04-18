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
#define MAX_THREADS 200 // Must be at least 2

/* Standard libs */
#include <cassert>
#include <iostream>
#include <pthread.h>
#include <vector>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/glcanvas.h>
#ifdef __WXMAC__
#include <GLUT/glut.h>
#else
#include <gl/GLU.h>
#endif


/* Project headers */
#include "Color.hpp"
#include "Config.hpp"
#include "Material.hpp"
#include "Perspective.hpp"
#include "Point.hpp"
#include "Sphere.hpp"
#include "Square.hpp"
#include "Triangle.hpp"
#include "Vector.hpp"

/* External headers */
#include "stb_image_write.h"
#include "tinyxml2.h"

typedef struct {
    int threadId;
    bool isSecondary;
    std::vector<Geometry *> * geometryArray;
    std::vector<Geometry *> * lightArray;
    unsigned char * imageArray;
} threadArgs;


using namespace std;

Color _ColorMapping(OBJECTS_FILE);
Config _Configuration(OBJECTS_FILE);
Perspective _Perspective(_Configuration, OBJECTS_FILE);

void setPixelColor(Vec3<unsigned char> color, Vec2<int> coordinate, unsigned char * array, int width) {
    
    int pos = (coordinate.y * 3 * width) + (coordinate.x * 3);
    
    array[pos] = color.x;
    array[++pos] = color.y;
    array[++pos] = color.z;
}

Vec3<unsigned char> getPixelColor(Vec2<int> coordinate, unsigned char * array, int width) {
    int pos = (coordinate.y * 3 * width) + (coordinate.x * 3);
    
    Vec3<unsigned char> color;
    color.x = array[pos];
    color.y = array[++pos];
    color.z = array[++pos];
    return color;
    
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
            Vec2<int> coordinate(j, i);
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
    // Grab the first child element in the file
    tinyxml2::XMLElement * objectParents = doc.FirstChildElement();
    
    // Go through the lights array and geometry array
    while(objectParents) {
        
        int isObject = 1;
        
        //Loop to find the objects or light parent element while the string is not "objects" or the size is non-zero
        while(objectParents && (isObject = strncmp(objectParents->Value(), "objects", 7)) && (strncmp(objectParents->Value(), "lights", 6)) ) {
            //cout << "Element text " << objectParents->Value() << endl;
            objectParents = objectParents->NextSiblingElement();
        }
        isObject = !isObject;
        
        if (!objectParents) {
            break;
        }
        
        
        tinyxml2::XMLElement * objectChild = objectParents->FirstChildElement();
        
        
        if(objectChild) { // object/light parsing
            
            // Iterate through the objects portion and add them to the geometry array
            while (objectChild) {
                
                // Triangle object
                if (!strncmp(objectChild->Value(), "triangle", 8)) {
                    Vec3<float> vertexA;
                    Vec3<float> vertexB;
                    Vec3<float> vertexC;
                    Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
                    Material mat = MATERIAL_NONE;
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
                                mat = MATERIAL_NONE;
                            }
                            else if (!strncmp(str.c_str(), "REFLECTIVE", 10)) {
                                mat = MATERIAL_REFLECTIVE;
                            }
                            else if (!strncmp(str.c_str(), "SPECULAR", 8)) {
                                mat = MATERIAL_SPECULAR;
                            }
                            else if (!strncmp(str.c_str(), "GLASS", 5)) {
                                mat = MATERIAL_GLASS;
                            }
                        }
                        tag = tag->NextSiblingElement();
                    }
                    
                    assert(vertexCount == 3);
                    
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
                    Material mat = MATERIAL_NONE;
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
                            
                            // Read the color and set the corresponding square color
                            str.assign(tag->GetText());
                            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
                            color = _ColorMapping.GetColor(str);
                            
                        }
                        else if (!strncmp(tag->Value(), "material", 8)) {
                            
                            // Read the material and set the corresponding material for the square
                            str.assign(tag->GetText());
                            std::transform(str.begin(), str.end(), str.begin(), ::toupper);
                            
                            // Assign the material
                            if (!strncmp(str.c_str(), "NONE", 4)) {
                                mat = MATERIAL_NONE;
                            }
                            else if (!strncmp(str.c_str(), "REFLECTIVE", 10)) {
                                mat = MATERIAL_REFLECTIVE;
                            }
                            else if (!strncmp(str.c_str(), "SPECULAR", 8)) {
                                mat = MATERIAL_SPECULAR;
                            }
                            else if (!strncmp(str.c_str(), "GLASS", 5)) {
                                mat = MATERIAL_GLASS;
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
                
                
                // Square object
                if (!strncmp(objectChild->Value(), "square", 6)) {
                    Vec3<float> vertexA;
                    Vec3<float> vertexB;
                    Vec3<float> vertexC;
                    Vec3<float> vertexD;
                    Vec3<unsigned char> color = _ColorMapping.GetColor("WHITE");
                    Material mat = MATERIAL_NONE;
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
                            else if (vertexCount == 2){
                                vertexC.SetValues((float)a, (float)b, (float)c);
                            }
                            else {
                                vertexD.SetValues((float)a, (float)b, (float)c);
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
                                mat = MATERIAL_NONE;
                            }
                            else if (!strncmp(str.c_str(), "REFLECTIVE", 10)) {
                                mat = MATERIAL_REFLECTIVE;
                            }
                            else if (!strncmp(str.c_str(), "SPECULAR", 8)) {
                                mat = MATERIAL_SPECULAR;
                            }
                            else if (!strncmp(str.c_str(), "GLASS", 5)) {
                                mat = MATERIAL_GLASS;
                            }
                        }
                        tag = tag->NextSiblingElement();
                    }
                    assert(vertexCount == 4);
                    
                    // Create a new triangle object and add it to the arrayj
                    if (isObject) {
                        geom.push_back(new Square(vertexA, vertexB, vertexC, vertexD, color, mat));
                    }
                    else {
                        lights.push_back(new Square(vertexA, vertexB, vertexC, vertexD, color, mat));
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
            
            // Corrected = 255 * (Image/255)^(1/2.2)
            coordinate.SetValues(j, i);
            color = getPixelColor(coordinate, imageArray, width);
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
    if(minHit->GetMaterial() == MATERIAL_REFLECTIVE) {
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
        Vec3<float> toLightSecondary = Vec3<float>::Normalize(randomPoint - (rayHit->GetHitLocation() + (rayHit->GetSecondaryNormal() * .00005f))); // Bump
        
        // See if the ray from the light source is in shadow or figure out the dot product between the two
        for (size_t j = 0; j < geometry.size(); j++) {
            std::shared_ptr<RayHit> tempHit;
            if ((tempHit = geometry.at(j)->Intersect(toLightRay, rayHit->GetHitLocation())) != nullptr || (tempHit = geometry.at(j)->Intersect(toLightSecondary, rayHit->GetHitLocation())) != nullptr) {
                
                //Make sure we didn't hit anything behind us
                if (tempHit->GetTime() > 0.0005f) {
                    intersected = true;
                }
            }
        }
        
        // We didn't hit anything so take the dot product
        if (!intersected) {
            float temp1 = toLightRay * rayHit->GetNormal();
            float temp2 = toLightRay * rayHit->GetSecondaryNormal();
            
            // Ambient light calculation (with multiple normals)
            if (temp1 > scale) {
                scale = temp1;
            }
            if (temp2 > scale) {
                scale = temp2;
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
    for(int i = args.threadId; i < _Configuration.GetPixelHeight(); i+=MAX_THREADS/2) {
        float heightOffset;
        if(!args.isSecondary) {
            heightOffset = _Perspective.GetImagePlane()->GetCorner().y - (_Perspective.GetUnitsPerHeightPixel() * (float)i);
        }
        else {
            heightOffset = _Perspective.GetSecondaryImagePlane()->GetCorner().y - (_Perspective.GetUnitsPerHeightPixel() * (float)i);
        }
        for(int j = 0; j < _Configuration.GetPixelLength(); j++) {
            Vec3<float> trueOffset;
            
            // Start at the corner of the image plane (x length)
            float xStart;
            if(args.isSecondary) {
                xStart = _Perspective.GetSecondaryImagePlane()->GetCorner().x;
            } else {
                xStart = _Perspective.GetImagePlane()->GetCorner().x;
            }
            trueOffset = Vec3<float>::vec3(xStart + (_Perspective.GetUnitsPerLengthPixel() * (float)j), heightOffset, _Perspective.GetImagePlane()->GetCorner().z);
            
            
            // Anti-aliasing
            if(_Configuration.IsAntialiased()) {
                std::vector<Vec3<unsigned char> > colorArray;
                for(int k = 0; k < 2; k++) {
                    Vec3<float> aliasHeightOffset(trueOffset.x, trueOffset.y - (_Perspective.GetUnitsPerHeightPixel() * ((float)k+1.f) ), trueOffset.z);
                    for (int l = 0; l < 2; l++) {
                        Vec3<float> aliasTotalOffset(aliasHeightOffset.x + (_Perspective.GetUnitsPerLengthPixel() * (float)l), aliasHeightOffset.y, aliasHeightOffset.z);
                        Vec3<float> tempRay = Vec3<float>::Normalize(aliasTotalOffset - _Perspective.GetCameraPosition());
                        std::shared_ptr<RayHit> rayHit = GetRay(tempRay, _Perspective.GetCameraPosition(), *(args.geometryArray), 0);
                        
                        // Push the colors back in the vector
                        if(rayHit == nullptr) {
                            colorArray.push_back(_ColorMapping.GetColor("BLACK"));
                        } else {
                            colorArray.push_back(CheckShadows(_Configuration.GetAmbientLight(), rayHit, *(args.geometryArray), *(args.lightArray)));
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
                
                Vec3<unsigned char> colorAvg(avg[0] / (unsigned char)colorArray.size(), avg[1] / (unsigned char)colorArray.size(), avg[2] / (unsigned char)colorArray.size());
                setPixelColor(colorAvg, coord, args.imageArray, _Configuration.GetPixelLength());
                
            } else {
                //Shoot a single ray
                Vec3<float> tempRay = Vec3<float>::Normalize(trueOffset - _Perspective.GetCameraPosition());
                std::shared_ptr<RayHit> rayHit = GetRay(tempRay, _Perspective.GetCameraPosition(), *(args.geometryArray), 0);
                Vec2<int> coord(j, i);
                
                // Set the pixel color
                if (rayHit == nullptr) {
                    setPixelColor(_ColorMapping.GetColor("BLACK"), coord, args.imageArray, _Configuration.GetPixelLength());
                } else {
                    Vec3<unsigned char> color = CheckShadows(_Configuration.GetAmbientLight(), rayHit, *(args.geometryArray), *(args.lightArray));
                    setPixelColor(color, coord, args.imageArray, _Configuration.GetPixelLength());
                }
            }
        }
    }
    pthread_exit(NULL);
    return NULL;
}

void RemoveRedChannel(unsigned char * imageArray, int length, int height) {
    for(int i = 0; i < height * length * 3; i++) {
        if(i % 3 != 0) {
            imageArray[i] = 0;
        }
    }
}

void RemoveCyanChannel(unsigned char * imageArray, int length, int height) {
    for(int i = 0; i < height * length * 3; i++) {
        if(i % 3 == 0) {
            imageArray[i] = 0;
        }
    }
}

void ConvertImageToGrayScale(unsigned char * imageArray, int length, int height) {
    //http://stackoverflow.com/questions/17615963/standard-rgb-to-grayscale-conversion
    for(int i = 0; i < length * height * 3; i+=3) {
        unsigned char y = 255 * (imageArray[i] / 255.f * 0.2126f +imageArray[i+1] / 255.f * 0.7152f + imageArray[i+2] / 255.f * 0.0722f);
        imageArray[i]   = y;
        imageArray[i+1] = y;
        imageArray[i+2] = y;
    }
}

int main(int argc, char * argv[]) {
    
    pthread_t pThreads[MAX_THREADS];
    bool background_gradient = false, hsl_interpolation = false;
    Vec3<float> gradientStart(0, 0, 0), gradientEnd(0, 0, 0);
    std::vector<Geometry *> geometryArray;
    std::vector<Geometry *> lightArray;
    
    // Read the config setting
    initGeometry(geometryArray, lightArray);
    
    // Debug --- Configuration information
    cout << "Configuration Information" << endl;
    cout << "Anti-aliasing: "  << _Configuration.IsAntialiased() << endl;
    cout << "Gamma correction: " << _Configuration.GammaCorrect() << endl;
    cout << "Normal correction: "  << _Configuration.NormalCorrect() << endl;
    cout << "Ambient light value: " << _Configuration.GetAmbientLight() << endl;
    cout << "Image length: " << _Configuration.GetPixelLength() << endl;
    cout << "Image height: "  << _Configuration.GetPixelHeight() << endl;
    
    
    // Debug --- PERSPECTIVE INFORMATION
    cout << "Perspective Information" << endl;
    Vec3<float> temp = _Perspective.GetImagePlane()->GetCorner();
    cout << "Image Plane " << temp.x << " " << temp.y << " " << temp.z << endl;
    cout << "Length of image plane " << _Perspective.GetImagePlane()->GetLength() << endl;
    cout << "Height of image plane " << _Perspective.GetImagePlane()->GetHeight() << endl;
    temp = _Perspective.GetCameraPosition();
    cout << "Camera Location " << temp.x << " " << temp.y << " " << temp.z << endl;
    cout << "Anaglyph mode " << _Perspective.GetAnaglyphMode() << endl;
    cout << "Intereye distance " << _Perspective.GetIntereyeDistance() << endl;
    cout << "Units per length " << _Perspective.GetUnitsPerLengthPixel() << endl;
    cout << "Units per height " << _Perspective.GetUnitsPerHeightPixel() << endl;
    
    
    unsigned char * imageArray0 = (unsigned char *) malloc(3 * _Configuration.GetPixelLength() * _Configuration.GetPixelHeight() * sizeof(unsigned char));
    unsigned char * imageArray1;
    
    // only allocate the second image array if we need to
    if(_Configuration.IsAnaglyph()) {
        imageArray1 = (unsigned char *) malloc(3 * _Configuration.GetPixelLength() * _Configuration.GetPixelHeight() * sizeof(unsigned char));
        if(!imageArray1) {
            cout << "Failed to allocate memory for the image array.  Exiting" << endl;
        }
    }
    
    // Make sure the image array was allocated correctly
    if(!imageArray0) {
        cout << "Failed to allocate memory for the image array.  Exiting" << endl;
        exit(1);
    }
    
    // Draw the gradient on the image
    if(background_gradient) {
        drawGradient(gradientStart, gradientEnd, _Configuration.GetPixelLength(), _Configuration.GetPixelHeight(), imageArray0, hsl_interpolation);
    }
    
    // argument structure for the threading
    threadArgs * tArgs = (threadArgs *) malloc(sizeof(threadArgs) * MAX_THREADS);
    tArgs[0].geometryArray = &geometryArray;
    tArgs[0].isSecondary = false;
    tArgs[0].imageArray = imageArray0;
    tArgs[0].lightArray = &lightArray;
    tArgs[0].threadId = 0;
    
    // Make sure the ImagePlane is set already
    assert(_Perspective.GetImagePlane() != nullptr);
    
    // Check the mode on the raytracing
    if(_Configuration.IsAnaglyph()) {
        
        // Start the first thread
        pthread_create(&pThreads[0], NULL, ShootRays, &tArgs[0]);
        
        // Copy the arguments for raytracing for the first image and start the pthreads
        for(int i = 1; i < (MAX_THREADS / 2) + 1; i++) {
            std::memcpy(&tArgs[i], &tArgs[0], sizeof(threadArgs));
            tArgs[i].threadId = i;
            pthread_create(&pThreads[i], NULL, ShootRays, &tArgs[i]);
        }
        
        // Switch the image plane on the anaglyph mode (parallel is separate image planes)
        if(_Perspective.GetAnaglyphMode() == ANAGLYPH_PARALLEL) {
            tArgs[MAX_THREADS / 2].isSecondary = true;
        } else {
            tArgs[MAX_THREADS / 2].isSecondary = false;
        }
        
        // Set the second image array and start the first row thread in the second image
        tArgs[MAX_THREADS / 2].imageArray = imageArray1;
        tArgs[MAX_THREADS / 2].threadId = 0;
        pthread_create(&pThreads[MAX_THREADS / 2], NULL, ShootRays, &tArgs[MAX_THREADS / 2]);
        
        // Copy the arguments for the second image array and start the pthreads
        for(int i = (MAX_THREADS / 2) + 1; i < MAX_THREADS; i++) {
            std::memcpy(&tArgs[i], &tArgs[MAX_THREADS/2], sizeof(threadArgs));
            tArgs[i].threadId = tArgs[i-1].threadId + 1;
            pthread_create(&pThreads[i], NULL, ShootRays, &tArgs[i]);
        }
        
        for(int i = 0; i < MAX_THREADS; i++) {
            pthread_join(pThreads[i], NULL);
        }
        
        
    } else { // normal raytracing
        
        // Start the first thread
        pthread_create(&pThreads[0], NULL, ShootRays, &tArgs[0]);
        
        // Copy the arguments for raytracing and start the other threads
        for(int i = 1; i < MAX_THREADS; i++) {
            std::memcpy(&tArgs[i], &tArgs[0], sizeof(threadArgs));
            tArgs[i].threadId = i;
            pthread_create(&pThreads[i], NULL, ShootRays, &tArgs[i]);
        }
        
        // Wait for the pthreads before continuing
        for(int i = 0; i < MAX_THREADS; i++) {
            pthread_join(pThreads[i], NULL);
        }
    }
    
    // Combine images if in anaglyph mode
    if(_Configuration.IsAnaglyph()) {
        // Convert images to grayscale
        ConvertImageToGrayScale(imageArray0, _Configuration.GetPixelLength(), _Configuration.GetPixelHeight());
        ConvertImageToGrayScale(imageArray1, _Configuration.GetPixelLength(), _Configuration.GetPixelHeight());
        
        
        // Remove red channel from the first image
        RemoveRedChannel(imageArray0, _Configuration.GetPixelLength(), _Configuration.GetPixelHeight());
        RemoveCyanChannel(imageArray1, _Configuration.GetPixelLength(), _Configuration.GetPixelHeight());
        
        unsigned char * anaglyphImage = (unsigned char *)malloc(sizeof(unsigned char) * _Configuration.GetPixelLength() * _Configuration.GetPixelHeight() * 3);
        if(!anaglyphImage) {
            cout << "Failed to allocate memory.  Exiting" << endl;
            exit(10);
        }
        
        // Copy the images on top of oneanother
        for(int i = 0; i < _Configuration.GetPixelLength(); i++) {
            for(int j = 0; j < _Configuration.GetPixelHeight(); j++) {
                Vec2<int> coord(i, j);
                Vec3<unsigned char> imageOneColor = getPixelColor(coord, imageArray0, _Configuration.GetPixelLength());
                Vec3<unsigned char> imageTwoColor = getPixelColor(coord, imageArray1, _Configuration.GetPixelLength());
                
                Vec3<unsigned char> newColor(min(imageOneColor.x + imageTwoColor.x, 255), min(imageOneColor.y + imageTwoColor.y, 255), min(imageOneColor.z + imageTwoColor.z, 255));
                
                setPixelColor(newColor, coord, anaglyphImage, _Configuration.GetPixelLength());
            }
        }
        
        // Gamma correction on images
        if(_Configuration.GammaCorrect()) {
            gammaCorrect(imageArray1, _Configuration.GetPixelHeight(), _Configuration.GetPixelLength());
            gammaCorrect(anaglyphImage, _Configuration.GetPixelHeight(), _Configuration.GetPixelLength());
        }
        
        
        // Write out the images
        stbi_write_png("output2.png", _Configuration.GetPixelLength(), _Configuration.GetPixelHeight(), 3, imageArray1, _Configuration.GetPixelLength()*3);
        stbi_write_png("anaglyph.png", _Configuration.GetPixelLength(), _Configuration.GetPixelHeight(), 3, anaglyphImage, _Configuration.GetPixelLength()*3);
        
        // Destroy the image arrays
        free(imageArray1);
        free(anaglyphImage);
    }
    
    
    
    // Gamma correction
    if(_Configuration.GammaCorrect()) {
        gammaCorrect(imageArray0, _Configuration.GetPixelHeight(), _Configuration.GetPixelLength());
    }
    
    // Write out the image(s)
    stbi_write_png("output1.png", _Configuration.GetPixelLength(), _Configuration.GetPixelHeight(), 3, imageArray0, _Configuration.GetPixelLength()*3);
    
    // Free memory
    DestroyGeometry(geometryArray);
    DestroyGeometry(lightArray);
    free(imageArray0);
    free(tArgs);
    return 0;
}
