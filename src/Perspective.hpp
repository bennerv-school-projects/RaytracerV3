#pragma once

#include "ImagePlane.hpp"
#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 3/4/17
 * Purpose: Contains Perspective information for the Raytracer
 */
class Perspective {
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
    Perspective(int length, int height, ImagePlane * imagePlane, Vec3<float> cam) {
        _pixelLength = length;
        _pixelHeight = height;
        _imagePlane = imagePlane;
        _cameraPosition = cam;
        
        // Calculate the units per pixel
        if(_imagePlane->GetLength() == 0) { // x doesn't change so as coords(i, j) progress i corresponds with the z direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane->GetWidth() / (float)_pixelLength);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane->GetHeight() / (float) _pixelHeight, 0.f);
        } else if(_imagePlane->GetWidth() == 0) { // z doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane->GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane->GetHeight() / (float) _pixelHeight, 0.f);
        } else { // y doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the z
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane->GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane->GetWidth() / (float) _pixelHeight);
        }
    }
    
    /*
     * Date: 3/8/17
     * Function Name: Perspective
     * Arguments:
     * Purpose: Constructor
     * Return Value: void (Constructor)
     */
    Perspective() {
        _imagePlane = nullptr;
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
    }
    
    
    /*
     * Date: 3/8/17
     * Function Name: GetAmbientLight
     * Arguments:
     *      void
     * Purpose: Gets the ambient light value
     * Return Value: float
     */
    float GetAmbientLight() {
        return _ambientLight;
    }
    
    /*
     * Date: 3/8/17
     * Function Name: GetAntiAliasing
     * Arguments:
     *      void
     * Purpose: Gets whether to anti-alias or not
     * Return Value: bool
     */
    bool GetAntiAliasing() {
        return _antiAliasing;
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
     * Return Value: ImagePlane *
     */
    ImagePlane * GetImagePlane() {
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
    Vec3<float> GetCameraPosition() {
        return _cameraPosition;
    }
    
    /*
     * Date: 3/8/17
     * Function Name: SetAmbientLight
     * Arguments:
     *      float - the ambient light value
     * Purpose: Sets the ambient light value to use
     * Return Value: void
     */
    void SetAmbientLight(float ambientLight) {
        _ambientLight = ambientLight;
    }
    
    /*
     * Date: 3/8/17
     * Function Name: SetAmbientLight
     * Arguments:
     *      bool - whether or not to anti-alias
     * Purpose: Sets whether to anti-alias or not
     * Return Value: void
     */
    void SetAntiAliasing(bool val) {
        _antiAliasing = val;
    }
    
    
    /*
     * Date: 3/8/17
     * Function Name: SetPixelHeight
     * Arguments:
     *      int - the height of the image in pixels
     * Purpose: Sets the pixel height of the image
     * Return Value: void
     */
    void SetPixelHeight(int pixelHeight) {
        _pixelHeight = pixelHeight;
    }
    
    /*
     * Date: 3/8/17
     * Function Name: SetPixelLength
     * Arguments:
     *      int - the length of the image in pixels
     * Purpose: Sets the pixel length of the image
     * Return Value: void
     */
    void SetPixelLength(int pixelLength) {
        _pixelLength = pixelLength;
    }
    
    /*
     * Date: 3/8/17
     * Function Name: SetImagePlane
     * Arguments:
     *      ImagePlane * - the image plane
     * Purpose: Sets the image plane and the pixel length untis as well
     * Return Value: void
     */
    void SetImagePlane(ImagePlane * imagePlane) {
        _imagePlane = imagePlane;
        
        
        // Calculate the units per pixel
        if(_imagePlane->GetLength() == 0) { // x doesn't change so as coords(i, j) progress i corresponds with the z direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane->GetWidth() / (float)_pixelLength);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane->GetHeight() / (float) _pixelHeight, 0.f);
        } else if(_imagePlane->GetWidth() == 0) { // z doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the y
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane->GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, _imagePlane->GetHeight() / (float) _pixelHeight, 0.f);
        } else { // y doesn't change so as coords(i, j) progress i corresponds with the x direction and j with the z
            _unitsPerLengthPixel = Vec3<float>::vec3(_imagePlane->GetLength() / (float)_pixelLength, 0.f, 0.f);
            _unitsPerHeightPixel = Vec3<float>::vec3(0.f, 0.f, _imagePlane->GetWidth() / (float) _pixelHeight);
        }
    }
    
    /*
     * Date: 3/8/17
     * Function Name: SetCameraPosition
     * Arguments:
     *      Vec3<float> - the world-space camera coordinates
     * Purpose: Sets the camera position
     * Return Value: void
     */
    void SetCameraPosition(Vec3<float> camPos) {
        _cameraPosition = camPos;
    }
    
    
    private:
    
    int _pixelLength; // the size of the image width in pixels
    int _pixelHeight; // the size of the image height in pixels
    
    float _ambientLight; // the ambient light value
    
    bool _antiAliasing; // whether the image is anti-aliased or not
    
    // For every length/height pixel we move (x, y, z) in the corresponding directions
    Vec3<float> _unitsPerLengthPixel; // the units per length pixel in the image (in meters)
    Vec3<float> _unitsPerHeightPixel; // the units per height pixel in the image (in meters)
    ImagePlane * _imagePlane; // the ImagePlane being projected upon
    Vec3<float> _cameraPosition; // the camera position
    
    
    
};
