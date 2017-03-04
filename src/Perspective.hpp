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
    
    private:
    
    int _pixelLength; // the size of the image width in pixels
    int _pixelHeight; // the size of the image height in pixels
    
    // For every length/height pixel we move (x, y, z) in the corresponding directions
    Vec3<float> _unitsPerLengthPixel; // the units per length pixel in the image (in meters)
    Vec3<float> _unitsPerHeightPixel; // the units per height pixel in the image (in meters)
    ImagePlane _imagePlane; // the ImagePlane being projected upon
    Vec3<float> _cameraPosition; // the camera position
    
    
    
};
