#pragma once

#include "ImagePlane.hpp"
#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 3/4/17
 * Purpose: Contains Perspective information for the Raytracer
 */
public Perspective {
    public :
    
    /*
     * Date: 3/4/17
     * Function Name: Perspective
     * Arguments:
     *      int         - the width of the image in pixels
     *      int         - the height of the image in pixels
     *      ImagePlane  - the ImagePlane the 'world' is being projected onto
     *      Vec3<float> - the camera position
     * Purpose: Constructor
     * Return Value: void (Constructor)
     */
    Perspective(int length, int height, ImagePlane imagePlane, Vec3<float> cam) {
        _pixelLength = length;
        _pixelHeight = height;
        _imagePlane = imagePlane;
        _cameraPosition = cam;
        
        // Calculate the units per pixel
        if(_imagePlane.GetLength() == 0) { // x doesn't change so as coords(i, j) progress i corresponds with the z direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane.GetWidth() / (float)_pixelLength);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane.GetHeight() / (float) _pixelHeight, 0.f);
        } else if(_imagePlane.GetWidth() == 0) { // z doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane.GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane.GetHeight() / (float) _pixelHeight, 0.f);
        } else { // y doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the z
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane.GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane.GetWidth() / (float) _pixelHeight);
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
        delete(_imagePlane);
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetPixelLength
     * Arguments:
     *      void
     * Purpose: Gets the length of the output image in pixels
     * Return Value: int
     */
    int GetPixelLength() {
        return _pixelLength;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetPixelHeight
     * Arguments:
     *      void
     * Purpose: Gets the height of the output image in pixels
     * Return Value: int
     */
    int GetPixelHeight() {
        return _pixelHeight;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetUnitsPerLengthPixel
     * Arguments:
     *      void
     * Purpose: Returns the offset in the x,y,z direction of the corresponding units per length pixel
     * Return Value: Vec3<float>
     */
    Vec3<float> GetUnitsPerLengthPixel() {
        return _unitsPerLengthPixel;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetUnitsPerHeightPixel
     * Arguments:
     *      void
     * Purpose: Returns the offset in the x,y,z direction of the corresponding units per height pixel
     * Return Value: Vec3<float>
     */
    Vec3<float> GetUnitsPerHeightPixel() {
        return _unitsPerHeightPixel;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetImagePlane
     * Arguments:
     *      void
     * Purpose: Gets the corresponding ImagePlane information about the image
     * Return Value: ImagePlane
     */
    ImagePlane GetImagePlane() {
        return _imagePlane;
    }
    
    /*
     * Date: 3/5/17
     * Function Name: GetCameraPosition
     * Arguments:
     *      void
     * Purpose: Gets the corresponding camera position in the world
     * Return Value: Vec3<float>
     */
    ImagePlane GetImagePlane() {
        return _imagePlane;
    }
    
    
    private:
    
    int _pixelLength; // the size of the image width in pixels
    int _pixelHeight; // the size of the image height in pixels
    
    // For every length/height pixel we move (x, y, z) in the corresponding directions
    Vec3<float> _unitsPerLengthPixel; // the units per length pixel in the image (in meters)
    Vec3<float> _unitsPerHeightPixel; // the units per height pixel in the image (in meters)
    ImagePlane * _imagePlane; // the ImagePlane being projected upon
    Vec3<float> _cameraPosition; // the camera position
    
    
    
};
