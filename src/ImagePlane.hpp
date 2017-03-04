#pragma once

#include <cassert>

#include "Vector.hpp"

/*
 * Author: Ben Vesel
 * Date: 3/4/17
 * Classname: ImagePlane
 * Purpose: An ImagePlane used to define the plane the image is projected onto or where the rays are shot from the camera to the image plane
 */
class ImagePlane : {
    
    public :
    
    /*
     * Date: 3/4/17
     * Function Name: Point
     * Arguments:
     *      Vec3<float> - the bottom left corner of the ImagePlane
     *      float       - the length of the ImagePlane
     *      float       - the width of the ImagePlane
     *      float       - the height of the ImagePlane
     * Purpose: Constructor
     * Return Value: void (Constructor)
     */
    ImagePlane(Vec3<float> corner, float length, float width, float height) {
        _corner = corner;
        _length = length;
        _width = width;
        _height = height;
        
        int zeroCount = 0;
        
        if(_length == 0) {
            zeroCount++;
        }
    
        if(_width == 0) {
            zeroCount++;
        }
        
        if(_height == 0) {
            zeroCount++;
        }
        
        // The ImagePlane should be parallel to one axis (as defined by the programmer in order not to do any fancy math in length/width/height pixels)
        assert(zeroCount == 1);
        
        // Make sure the length/width/heights are greater than zero (otherwise we will have negative units for pixel length/height)
        assert(_length >= 0);
        assert(_width >= 0);
        assert(_height >= 0);
        
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetCorner
     * Arguments:
     *      void
     * Purpose: Gets the lower lefthand corner of the ImagePlane
     * Return Value: Vec3<float>
     */
    Vec3<float> GetCorner() {
        return _corner;
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetHeight
     * Arguments:
     *      void
     * Purpose: Gets the height of the ImagePlane
     * Return Value: float
     */
    float GetHeight() {
        return _height;
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetLength
     * Arguments:
     *      void
     * Purpose: Gets the length of the ImagePlane
     * Return Value: float
     */
    float GetLength() {
        return _length;
    }
    
    /*
     * Date: 3/4/17
     * Function Name: GetWidth
     * Arguments:
     *      void
     * Purpose: Gets the width of the ImagePlane
     * Return Value: float
     */
    float GetWidth() {
        return _width;
    }

    

    private :
    
    Vec3<float> _corner; // bottom left corner
    float _length; // +/- x value
    float _width; // +/- z value
    float _height; // +/- y value
};
