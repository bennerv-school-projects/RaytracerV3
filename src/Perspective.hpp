#pragma once

#include "ImagePlane.hpp"
#include "tinyxml2.h"
#include "Vector.hpp"
#include "Config.hpp"

#include <string>

// The type of anaglyph images we can render
enum anaglyph_type {
    ANAGLYPH_PARALLEL,
    ANAGLYPH_CONVERGE,
    ANAGLYPH_NONE
};

/*
 * Author: Ben Vesel
 * Date: 3/4/17
 * Purpose: Contains Perspective information for the Raytracer
 */
class Perspective {
    public :
    
    /*
     * Date: 3/8/17
     * Function Name: Perspective
     * Arguments:
     * Purpose: Constructor
     * Return Value: void (Constructor)
     */
    Perspective(Config config, std::string fileName) : _unitsPerLengthPixel(0), _unitsPerHeightPixel(0), _imagePlane(nullptr), _secondaryImagePlane(nullptr), _cameraPosition(0, 0, 0), _intereyeDistance(0), _anaglyphMode(ANAGLYPH_NONE) {
        
        
        tinyxml2::XMLDocument doc;
        doc.LoadFile(fileName.c_str());
        
        // Parse the xml document
        if (doc.Error()) {
            std::cout << "There was an error parsing " << fileName << std::endl;
            doc.PrintError();
            exit(1);
        }
        
        // Iterate through the xml elements for the configuration section
        tinyxml2::XMLElement * rootElement = doc.FirstChildElement();
        while (rootElement && strncmp(rootElement->Value(), "image_plane", 11)) {
            rootElement = rootElement->NextSiblingElement();
        }
        
        
        if(rootElement) {
            
            tinyxml2::XMLElement * imagePlaneElement = rootElement->FirstChildElement();
            
            Vec3<float> corner(-1, -1, -1);
            float length = -1, height = -1;
            bool anaglyphSettings = false;
            float intereyeDistance = -1;
            anaglyph_type anaglyphMode = ANAGLYPH_NONE;
            
            // Go through each of the image plane information portions
            while(imagePlaneElement) {
                
                // Parse the camera location
                if(!strncmp(imagePlaneElement->Value(), "camera", 6)) {
                    tinyxml2::XMLElement * cameraLocationElement = imagePlaneElement->FirstChildElement();
                    
                    if(cameraLocationElement) {
                        double x = -1, y = -1, z = -1;
                        
                        cameraLocationElement->QueryDoubleAttribute("x", &x);
                        cameraLocationElement->QueryDoubleAttribute("y", &y);
                        cameraLocationElement->QueryDoubleAttribute("z", &z);
                        
                        // Set the camera postition
                        _cameraPosition = Vec3<float>::vec3(x, y, z);
                    }
                }
                else if(!strncmp(imagePlaneElement->Value(), "corner", 6)) {
                    double x = -1, y = -1, z = -1;
                    
                    // Parse the corners
                    imagePlaneElement->QueryDoubleAttribute("x", &x);
                    imagePlaneElement->QueryDoubleAttribute("y", &y);
                    imagePlaneElement->QueryDoubleAttribute("z", &z);
                    
                    corner = Vec3<float>::vec3((float)x, (float)y, (float)z);
                }
                else if(!strncmp(imagePlaneElement->Value(), "length", 6)) {
                    length = (float)atof(imagePlaneElement->GetText());
                }
                else if(!strncmp(imagePlaneElement->Value(), "height", 6)) {
                    height = (float)atof(imagePlaneElement->GetText());
                }
                else if(!strncmp(imagePlaneElement->Value(), "anaglyph", 8)) {
                    anaglyphSettings = true;
                    // Make sure it is an anaglyph image
                    if(config.IsAnaglyph()) {
                        tinyxml2::XMLElement * anaglyphElement = imagePlaneElement->FirstChildElement();
                        
                        // Go through each of the portions of anaglyphElement
                        while(anaglyphElement) {
                            
                            // Parse the mode portion
                            if(!strncmp(anaglyphElement->Value(), "mode", 4)) {
                                std::string mode(anaglyphElement->GetText());
                                std::transform(mode.begin(), mode.end(), mode.begin(), ::toupper);
                                
                                
                                // Set the anaglyph mode
                                if(!strncmp(mode.c_str(), "PARALLEL", 8)) {
                                    anaglyphMode = ANAGLYPH_PARALLEL;
                                }
                                else if(!strncmp(mode.c_str(), "CONVERGE", 8) || !strncmp(mode.c_str(), "CONVERGING", 10)) {
                                    anaglyphMode = ANAGLYPH_CONVERGE;
                                }
                            }
                            
                            // Parse the intereye distance portion
                            else if(!strncmp(anaglyphElement->Value(), "intereye_distance", 17)) {
                                intereyeDistance = (float)atof(anaglyphElement->GetText());
                            }
                            anaglyphElement = anaglyphElement->NextSiblingElement();
                        }
                    }
                }
                imagePlaneElement = imagePlaneElement->NextSiblingElement();
            }
            
            // Make sure if anaglyph mode is enabled that they specified the intereye distance and mode
            if(!anaglyphSettings && config.IsAnaglyph()) {
                std::cout << "You must have an anaglyph portion in the xml in order to process an anaglyph image" << std::endl;
                exit(11);
            }
            
            // Make sure anaglyph mode is configured and the intereyeDistance is greater than zero
            if(config.IsAnaglyph() && (anaglyphMode == ANAGLYPH_NONE || intereyeDistance <= 0)) {
                std::cout << "You must configure an anaglyph mode if you enable anaglyphs, or intereye distance must be greater than 0" << std::endl;
                exit(12);
            }
            
            // Create the image plane
            _imagePlane = new ImagePlane(corner, length, height);
            
            // Set up the secondary image plane and anaglyph modes
            if(config.IsAnaglyph() ) {
                _anaglyphMode = anaglyphMode;
                _intereyeDistance = intereyeDistance;
                
                // Set the secondary image plane
                Vec3<float> newCorner(corner.x + _intereyeDistance, corner.y, corner.z);
                _secondaryImagePlane = new ImagePlane(newCorner, length, height);
            }
            
            // Set the units per pixel lengths
            _unitsPerLengthPixel = _imagePlane->GetLength() / config.GetPixelLength();
            _unitsPerHeightPixel = _imagePlane->GetHeight() / config.GetPixelHeight();
        }
        
        // Make sure that we set our pixelLength units
        if(_imagePlane == nullptr) {
            std::cout << "Didn't find an image_plane section in the xml file.  Exiting" << std::endl;
            exit(12);
        }
    }
    
    
    /*
     * Date: 3/4/17
     * Function Name: ~Perspective
     * Arguments:
     * Purpose: Destructor
     * Return Value: void (Destructor)
     */
    ~Perspective() {
        if(_imagePlane != nullptr) {
            delete(_imagePlane);
        }
        if(_secondaryImagePlane != nullptr) {
            delete(_secondaryImagePlane);
        }
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetUnitsPerLengthPixel
     * Arguments:
     *      void
     * Purpose: Returns the offset in the x direction of the corresponding units per length pixel
     * Return Value: float
     */
    float GetUnitsPerLengthPixel() {
        return _unitsPerLengthPixel;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetUnitsPerHeightPixel
     * Arguments:
     *      void
     * Purpose: Returns the offset in the y direction of the corresponding units per height pixel
     * Return Value: float
     */
    float GetUnitsPerHeightPixel() {
        return _unitsPerHeightPixel;
    }
    
    /*
     * Date: 4/5/17
     * Function Name: GetIntereyeDistance
     * Arguments:
     *      void
     * Purpose: Returns the intereye distance for an anaglyph image
     * Return Value: float
     */
    float GetIntereyeDistance() {
        return _intereyeDistance;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetImagePlane
     * Arguments:
     *      void
     * Purpose: Gets the corresponding ImagePlane information about the image
     * Return Value: ImagePlane *
     */
    ImagePlane * GetImagePlane() {
        return _imagePlane;
    }
    
    /*
     * Date: 4/5/17
     * Function Name: GetSecondaryImagePlane
     * Arguments:
     *      void
     * Purpose: Gets the corresponding ImagePlane information about the image
     * Return Value: ImagePlane *
     */
    ImagePlane * GetSecondaryImagePlane() {
        return _secondaryImagePlane;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetCameraPosition
     * Arguments:
     *      void
     * Purpose: Gets the corresponding camera position in the world
     * Return Value: Vec3<float>
     */
    Vec3<float> GetCameraPosition() {
        return _cameraPosition;
    }
    
    /*
     * Date: 4/14/17
     * Function Name: GetAnaglyphMode
     * Arguments:
     *      void
     * Purpose: Gets the anaglyph mode (assuming anaglyph mode is activated)
     * Return Value: anaglyph_type
     */
    anaglyph_type GetAnaglyphMode() {
        return _anaglyphMode;
    }
    
    private:
    
    // For every length/height pixel we move (x, y, z) in the corresponding directions
    float _unitsPerLengthPixel; // the units per length pixel in the image (in meters)
    float _unitsPerHeightPixel; // the units per height pixel in the image (in meters)
    ImagePlane * _imagePlane; // the ImagePlane being projected upon
    ImagePlane * _secondaryImagePlane; // the secondary ImagePlane being projected upon (anaglyph mode)
    Vec3<float> _cameraPosition; // the camera position
    float _intereyeDistance; // the intereye distance for anaglyph images
    anaglyph_type _anaglyphMode;
    
    
    
};
